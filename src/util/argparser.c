#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"

/* --- Preprocessor directives ---------------------------------------------- */

#define STC_ARG_DESC_PADDING   10
#define STC_ARG_DESC_MAX_WIDTH 70

#define STC_ARG_IS_POSITIONAL(arg) (!((arg).shortopt || (arg).longopt))
#define STC_ARG_IS_STR(argtype)    ((argtype) == STC_ARG_STR)
#define STC_ARG_IS_BOOL(argtype)   ((argtype) == STC_ARG_BOOL)
#define STC_ARG_IS_CUSTOM(argtype) ((argtype) == STC_ARG_CUSTOM)

#define STC_ARG_CHECK_OPTS(shortopt, longopt)                                  \
    do {                                                                       \
        if (!(shortopt || longopt)) {                                          \
            fputs("ERROR: values for both shortopt and longopt not set\n",     \
                  stderr);                                                     \
            exit(EXIT_FAILURE);                                                \
        } else if ((shortopt) && (shortopt)[0] != '-') {                       \
            fprintf(                                                           \
                stderr,                                                        \
                "ERROR: shortopt does not start with '-': shortopt='%s'\n",    \
                (shortopt));                                                   \
            exit(EXIT_FAILURE);                                                \
        } else if ((longopt) && (longopt)[0] != '-' || (longopt)[1] != '-') {  \
            fprintf(stderr,                                                    \
                    "ERROR: longopt does not start with '--': longopt='%s'\n", \
                    (longopt));                                                \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

#define STC_ARG_SHORTOPT_MATCHES(found, opt, len, arg) \
    ((opt = arg.shortopt) && (len = is_prefixed(found, opt)) && !found[len])
#define STC_ARG_LONGOPT_MATCHES(found, opt, len, arg)          \
    ((opt = arg.longopt) && (len = is_prefixed(found, opt)) && \
     (found[len] == '=' || found[len] == '\0'))
#define STC_ARG_OPT_MATCHES(found, opt, len, arg)      \
    (STC_ARG_SHORTOPT_MATCHES(found, opt, len, arg) || \
     STC_ARG_LONGOPT_MATCHES(found, opt, len, arg))

#define STC_ARGS_USAGE(argparser, stream, program)           \
    (argparser)->usage ? (argparser)->usage(stream, program) \
                       : stc_argparser_print_usage(argparser, program, stream)

#define GET_ARG_NAME(opt, apl)                                                \
    do {                                                                      \
        if ((apl)->is_subcmd)                                                 \
            (opt) = (apl)->subcmd->name;                                      \
        else if (apl->arg.shortopt)                                           \
            (opt) = (apl)->arg.shortopt;                                      \
        else                                                                  \
            (opt) =                                                           \
                (apl)->arg.longopt ? (apl)->arg.longopt : (apl)->arg.valname; \
    } while (0)

/* --- Type definitions ----------------------------------------------------- */

typedef struct stc_subargparser  StcSubArgParser;
typedef struct stc_argparse_list StcArgParseList;

typedef struct {
    StcArgType     type;        /**< type of the argument                     */
    const char    *shortopt;    /**< short name string (must start with '-')  */
    const char    *longopt;     /**< long name string (must start with '--')  */
    const char    *valname;     /**< optional name of value to show in usage  */
    const char    *description; /**< optional description of the argument     */
    void          *out;         /**< optional pointer to store argument value */
    const void    *def;         /**< optional default value of the argument
                                     (must be the string of the default value
                                     for the convert function for custom
                                     arguments)                               */
    StcArgConvert *convert; /**< optional function to convert custom argument */
} StcArg;

struct stc_subargparser {
    const char   *cmd;         /**< name of command to trigger the subparser  */
    const char   *description; /**< optional description of the subcommand    */
    StcArgParser *argparser;   /**< argument parser to be used for subparsing */

    StcSubArgParser *prev; /**< previous subparser in subparser list          */
    StcSubArgParser *next; /**< next subparser in subparser list              */
};

struct stc_subargparsers {
    const char  *name; /**< name of subcommand to show in usage               */
    const char **out;  /**< optional pointer to store subcommand value        */

    StcSubArgParser *sentinel; /**< circly linked list of subparsers          */
};

struct stc_argparse_list {
    int is_subcmd; /**< whether this argparse item is a subcommand            */
    int idx;       /**< index into argparse list                              */

    union {
        StcArg            arg;    /**< argument specifier (is_subcmd == 0)    */
        StcSubArgParsers *subcmd; /**< subparser to parse (is_subcmd != 0)    */
    };

    StcArgParseList *prev; /**< previous item in argparse list                */
    StcArgParseList *next; /**< next item in argparse list                    */
};

struct stc_argparser {
    StcArgParserUsage *usage;    /**< optional custom print usage function    */
    StcArgParseList   *sentinel; /**< circly linked list of argparse items    */
};

/* --- Helper function prototypes ------------------------------------------- */

static void stc_argparser_add_arg(StcArgParser  *self,
                                  StcArgType     argtype,
                                  const char    *shortopt,
                                  const char    *longopt,
                                  const char    *valname,
                                  const char    *description,
                                  void          *out,
                                  const void    *def,
                                  StcArgConvert *convert);
static int  _stc_argparser_parse(const StcArgParser *self,
                                 int                 argc,
                                 const char        **argv,
                                 int                *idx,
                                 int                 end);
static int  stc_subargparsers_parse(const StcSubArgParsers *self,
                                    const StcArgParser     *parent,
                                    const char             *program,
                                    int                     argc,
                                    const char            **argv,
                                    int                    *idx,
                                    int                     end);
static void stc_subargparser_print_usage(const StcSubArgParsers *self,
                                         FILE                   *stream);
static void stc_arg_print_usage(FILE *stream, const StcArg *arg);
static void
stc_arg_print_opt_usage(FILE *stream, const StcArg *arg, int shortlen);
static void stc_arg_print_description(FILE *stream, const char *description);
static int
stc_arg_process(const char *found, const StcArg *arg, const char *opt);
static int    stc_arg_memcpy(void          *dst,
                             const void    *src,
                             StcArgConvert *convert,
                             StcArgType     type);
static size_t is_prefixed(const char *str, const char *prefix);

/* --- Public function definitions ------------------------------------------ */

StcArgParser *stc_argparser_new(StcArgParserUsage *usage)
{
    StcArgParser *argparser = malloc(sizeof(*argparser));

    argparser->usage          = usage;
    argparser->sentinel       = calloc(1, sizeof(*argparser->sentinel));
    argparser->sentinel->prev = argparser->sentinel->next = argparser->sentinel;

    return argparser;
}

void stc_argparser_free(StcArgParser *self)
{
    StcArgParseList *apl, *apl_next;
    StcSubArgParser *sap, *sap_next;

    for (apl = self->sentinel->next; apl != self->sentinel; apl = apl_next) {
        apl_next = apl->next;
        if (apl->is_subcmd) {
            for (sap = apl->subcmd->sentinel->next;
                 sap != apl->subcmd->sentinel; sap = sap_next) {
                sap_next = sap->next;
                stc_argparser_free(sap->argparser);
            }
            free(apl->subcmd->sentinel);
        }
        free(apl);
    }

    free(self->sentinel);
    free(self);
}

void stc_argparser_add_str_option(StcArgParser *self,
                                  const char   *shortopt,
                                  const char   *longopt,
                                  const char   *valname,
                                  const char   *description,
                                  const char  **out,
                                  const char   *def)
{
    STC_ARG_CHECK_OPTS(shortopt, longopt);
    stc_argparser_add_arg(self, STC_ARG_STR, shortopt, longopt, valname,
                          description, out, def, NULL);
}

void stc_argparser_add_bool_option(StcArgParser *self,
                                   const char   *shortopt,
                                   const char   *longopt,
                                   const char   *description,
                                   int          *out,
                                   int           negate)
{
    static int def_for_negate[1];

    STC_ARG_CHECK_OPTS(shortopt, longopt);
    stc_argparser_add_arg(self, STC_ARG_BOOL, shortopt, longopt, NULL,
                          description, out, negate ? def_for_negate : NULL,
                          NULL);
}

void stc_argparser_add_custom_option(StcArgParser  *self,
                                     const char    *shortopt,
                                     const char    *longopt,
                                     const char    *valname,
                                     const char    *description,
                                     void          *out,
                                     const char    *def,
                                     StcArgConvert *convert)
{
    STC_ARG_CHECK_OPTS(shortopt, longopt);
    stc_argparser_add_arg(self, STC_ARG_CUSTOM, shortopt, longopt, valname,
                          description, out, def, convert);
}

void stc_argparser_add_str_argument(StcArgParser *self,
                                    const char   *argname,
                                    const char   *description,
                                    const char  **out)
{
    stc_argparser_add_arg(self, STC_ARG_STR, NULL, NULL, argname, description,
                          out, NULL, NULL);
}

void stc_argparser_add_custom_argument(StcArgParser  *self,
                                       const char    *argname,
                                       const char    *description,
                                       void          *out,
                                       StcArgConvert *convert)
{
    stc_argparser_add_arg(self, STC_ARG_CUSTOM, NULL, NULL, argname,
                          description, out, NULL, convert);
}

StcSubArgParsers *stc_argparser_add_subparsers(StcArgParser *self,
                                               const char   *name,
                                               const char  **out)
{
    StcArgParseList *apl = malloc(sizeof(*apl));

    apl->is_subcmd              = 1;
    apl->idx                    = self->sentinel->idx++;
    apl->subcmd                 = malloc(sizeof(*apl->subcmd));
    apl->subcmd->name           = name;
    apl->subcmd->out            = out;
    apl->subcmd->sentinel       = malloc(sizeof(*apl->subcmd->sentinel));
    apl->subcmd->sentinel->prev = apl->subcmd->sentinel->next =
        apl->subcmd->sentinel;

    apl->prev       = self->sentinel->prev;
    apl->next       = self->sentinel;
    apl->prev->next = apl->next->prev = apl;

    return apl->subcmd;
}

void stc_argparser_parse(const StcArgParser *self, int argc, const char **argv)
{
    int exit_code, idx = 0;

    if ((exit_code = _stc_argparser_parse(self, argc, argv, &idx, argc)) < 0)
        exit(EXIT_FAILURE);
    else if (exit_code > 0)
        exit(EXIT_SUCCESS);

    stc_argparser_check_for_help(self, argc, argv, idx);
    if (idx < argc && (strcmp(argv[idx], "--") != 0 || ++idx < argc)) {
        fprintf(stderr, "ERROR: unrecognised argument '%s'\n", argv[idx]);
        STC_ARGS_USAGE(self, stderr, argv[0]);
        exit(EXIT_FAILURE);
    }
}

void stc_argparser_print_usage(const StcArgParser *self,
                               const char         *program,
                               FILE               *stream)
{
    int                    len, has_arg, has_opt, shortlen = 0;
    const char            *name;
    const StcArgParseList *apl;

    /* check if there are any options or arguments */
    has_arg = has_opt = 0;
    for (apl = self->sentinel->next;
         apl != self->sentinel && (!has_arg || !has_opt); apl = apl->next)
        if (apl->is_subcmd || STC_ARG_IS_POSITIONAL(apl->arg))
            has_arg = 1;
        else
            has_opt = 1;

    /* print top usage line */
    fprintf(stream, "Usage: %s%s", program, has_opt ? " [OPTIONS]" : "");
    for (apl = self->sentinel->next; apl != self->sentinel; apl = apl->next) {
        if (apl->is_subcmd || STC_ARG_IS_POSITIONAL(apl->arg)) {
            name = apl->is_subcmd ? apl->subcmd->name : apl->arg.valname;
            fprintf(stream, " %s", name);
            if (apl->is_subcmd) fputs(" ...", stream);
        } else if (apl->arg.shortopt &&
                   (len = strlen(apl->arg.shortopt)) > shortlen) {
            shortlen = len;
        }
    }
    fputc('\n', stream);

#define PRINT_NEWLINE(filestream, should_print) \
    do {                                        \
        if ((should_print))                     \
            fputc('\n', (filestream));          \
        else                                    \
            (should_print) = 1;                 \
    } while (0)
    /* print descriptions of arguments */
    if (has_arg) {
        len = 0; /* use len as flag to indicate whether first argument done */
        fputs("\nArguments:\n", stream);
        for (apl = self->sentinel->next; apl != self->sentinel;
             apl = apl->next) {
            if (apl->is_subcmd) {
                PRINT_NEWLINE(stream, len);
                stc_subargparser_print_usage(apl->subcmd, stream);
            } else if (STC_ARG_IS_POSITIONAL(apl->arg)) {
                PRINT_NEWLINE(stream, len);
                stc_arg_print_usage(stream, &apl->arg);
            }
        }
    }

    /* print descriptions of options */
    if (has_opt) {
        len = 0; /* use len as flag to indicate whether first option done */
        fputs("\nOptions:\n", stream);
        for (apl = self->sentinel->next; apl != self->sentinel; apl = apl->next)
            if (!apl->is_subcmd && !STC_ARG_IS_POSITIONAL(apl->arg)) {
                PRINT_NEWLINE(stream, len);
                stc_arg_print_opt_usage(stream, &apl->arg, shortlen);
            }
    }
#undef PRINT_NEWLINE
}

void stc_argparser_check_for_help(const StcArgParser *self,
                                  int                 argc,
                                  const char        **argv,
                                  int                 arg_idx)
{
    int i;

    for (i = arg_idx; i < argc && strcmp(argv[i], "--") != 0; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            STC_ARGS_USAGE(self, stdout, argv[0]);
            exit(EXIT_SUCCESS);
        }
    }
}

StcArgParser *stc_subargparsers_add_argparser(StcSubArgParsers  *self,
                                              const char        *cmd,
                                              const char        *description,
                                              StcArgParserUsage *usage)
{
    StcSubArgParser *sap = malloc(sizeof(*sap));

    sap->cmd         = cmd;
    sap->description = description;
    sap->argparser   = stc_argparser_new(usage);

    sap->prev       = self->sentinel->prev;
    sap->next       = self->sentinel;
    sap->prev->next = sap->next->prev = sap;

    return sap->argparser;
}

/* --- Helper function definitions ------------------------------------------ */

static void stc_argparser_add_arg(StcArgParser  *self,
                                  StcArgType     argtype,
                                  const char    *shortopt,
                                  const char    *longopt,
                                  const char    *valname,
                                  const char    *description,
                                  void          *out,
                                  const void    *def,
                                  StcArgConvert *convert)
{
    StcArgParseList *apl = malloc(sizeof(*apl));

    apl->idx             = self->sentinel->idx++;
    apl->arg.type        = argtype;
    apl->arg.shortopt    = shortopt;
    apl->arg.longopt     = longopt;
    apl->arg.valname     = valname;
    apl->arg.description = description;
    apl->arg.out         = out;
    apl->arg.def         = def;
    apl->arg.convert     = convert;

    apl->prev       = self->sentinel->prev;
    apl->next       = self->sentinel;
    apl->prev->next = apl->next->prev = apl;
}

static int _stc_argparser_parse(const StcArgParser *self,
                                int                 argc,
                                const char        **argv,
                                int                *idx,
                                int                 end)
{
    size_t                  len;
    int                     i, j;
    int                     arg_idx = 0, opt_idx = 0;
    int                     nargs = 0, nopts = 0;
    int                     done_opts = 0, exit_code = 0;
    int                    *argset;
    const char             *found, *opt, *program = argv[*idx];
    const StcArgParseList **args, **opts;
    const StcArgParseList  *apl;

    /* determine the number of positional and optional arguments there are */
    for (apl = self->sentinel->next; apl != self->sentinel; apl = apl->next)
        if (apl->is_subcmd || STC_ARG_IS_POSITIONAL(apl->arg))
            nargs++;
        else
            nopts++;

    /* populate the positional and optional argument indices arrays */
    argset = calloc(self->sentinel->idx, sizeof(*argset));
    args = malloc(nargs * sizeof(*args)); // NOLINT(bugprone-sizeof-expression)
    opts = malloc(nopts * sizeof(*opts)); // NOLINT(bugprone-sizeof-expression)
    for (apl = self->sentinel->next, i = j = 0; apl != self->sentinel;
         apl = apl->next)
        if (apl->is_subcmd || STC_ARG_IS_POSITIONAL(apl->arg))
            args[i++] = apl;
        else
            opts[j++] = apl;

    /* loop through the command-line arguments and check for specified args */
    for (++*idx; *idx < end; ++*idx) {
        found = argv[*idx];

        if (found[0] == '-' && !done_opts) {
            /* check for '--' to indicate done with options */
            if (strcmp(found, "--") == 0) {
                done_opts = 1;
                goto unrecognised; /* not unrecognised, but to keep order */
            }

            /* check for non-positional argument matches */
            for (i = opt_idx; i < nopts; i++) {
                apl = opts[i];
                if (STC_ARG_OPT_MATCHES(found, opt, len, apl->arg)) {
                    found           += len;
                    opts[i]          = opts[opt_idx];
                    opts[opt_idx++]  = apl;
                    break;
                }
            }
            if (i >= nopts) {
                if (strcmp(found, "-h") == 0 || strcmp(found, "--help") == 0) {
                    STC_ARGS_USAGE(self, stdout, program);
                    exit_code = 1;
                    goto done;
                } else {
                    goto unrecognised;
                }
            }
        } else {
            /* check if we still have positional arguments to match */
            if (arg_idx >= nargs) {
            unrecognised:
                /* process unrecognised argument */
                memmove(&argv[*idx], &argv[*idx + 1],
                        (argc - *idx - 1) * sizeof(*argv));
                argv[argc - 1] = found;
                end--;
                --*idx;
                continue;
            }

            apl = args[arg_idx++];
            opt = apl->arg.valname;
        }

        /* process the command-line argument against specification */
        if (apl->is_subcmd) {
            exit_code = stc_subargparsers_parse(apl->subcmd, self, program,
                                                argc, argv, idx, end);
            if (exit_code)
                goto done;
            else
                --*idx;
        } else {
            if (!STC_ARG_IS_POSITIONAL(apl->arg) && *found == '\0')
                found = *idx + 1 < end ? argv[*idx + 1] : NULL;
            if ((exit_code = stc_arg_process(found, &apl->arg, opt)) > 0) {
                ++*idx;
            } else if (exit_code < 0) {
                STC_ARGS_USAGE(self, stderr, program);
                goto done;
            }
        }
        argset[apl->idx] = 1;
    }

    /* check if any arguments were not found/set */
    for (apl = self->sentinel->next; apl != self->sentinel; apl = apl->next) {
        if (argset[apl->idx]) continue;
        if (apl->is_subcmd ||
            (apl->arg.def == NULL && !STC_ARG_IS_BOOL(apl->arg.type))) {
            GET_ARG_NAME(opt, apl);
            fprintf(stderr, "ERROR: argument '%s' not specified\n", opt);
            STC_ARGS_USAGE(self, stderr, program);
            exit_code = -1;
            goto done;
        }
        if (apl->arg.out && !stc_arg_memcpy(apl->arg.out, apl->arg.def,
                                            apl->arg.convert, apl->arg.type)) {
            GET_ARG_NAME(opt, apl);
            fprintf(stderr,
                    "ERROR: failed to set default value for argument '%s' due "
                    "to conversion failure\n",
                    opt);
            STC_ARGS_USAGE(self, stderr, program);
            exit_code = -1;
            goto done;
        }
    }

done:
    free(argset);
    free(args);
    free(opts);
    return exit_code;
}

static int stc_subargparsers_parse(const StcSubArgParsers *self,
                                   const StcArgParser     *parent,
                                   const char             *program,
                                   int                     argc,
                                   const char            **argv,
                                   int                    *idx,
                                   int                     end)
{
    int                    exit_code = -1;
    const char            *subcmd    = argv[*idx];
    const StcSubArgParser *sap;

    for (sap = self->sentinel->next; sap != self->sentinel; sap = sap->next) {
        if (strcmp(subcmd, sap->cmd) == 0) {
            exit_code =
                _stc_argparser_parse(sap->argparser, argc, argv, idx, end);
            if (self->out) *self->out = sap->cmd;
            break;
        }
    }

    if (sap == self->sentinel) {
        fprintf(
            stderr,
            "ERROR: unrecognised subcommand choice '%s' for subcommand '%s'\n",
            subcmd, self->name);
        STC_ARGS_USAGE(parent, stderr, program);
        exit_code = -1;
    }

    return exit_code;
}

static void stc_subargparser_print_usage(const StcSubArgParsers *self,
                                         FILE                   *stream)
{
    const StcSubArgParser *sap;

    fprintf(stream, "  %s {", self->name);
    for (sap = self->sentinel->next; sap != self->sentinel; sap = sap->next) {
        fputs(sap->cmd, stream);
        if (sap->next != self->sentinel) fputs(", ", stream);
    }
    fputs("}\n", stream);

    for (sap = self->sentinel->next; sap != self->sentinel; sap = sap->next) {
        fprintf(stream, "    %s\n", sap->cmd);
        stc_arg_print_description(stream, sap->description);
    }
}

static void stc_arg_print_usage(FILE *stream, const StcArg *arg)
{
    fprintf(stream, "  %s\n", arg->valname);
    stc_arg_print_description(stream, arg->description);
}

static void
stc_arg_print_opt_usage(FILE *stream, const StcArg *arg, int shortlen)
{
    const char *shortopt, *longopt;

    shortopt = arg->shortopt ? arg->shortopt : "";
    longopt  = arg->longopt ? arg->longopt : "";
    fprintf(stream, "  %-*s%2s%s", shortlen, shortopt,
            arg->shortopt && arg->longopt ? ", " : "", longopt);

    if (!STC_ARG_IS_BOOL(arg->type))
        fprintf(stream, " <%s>", arg->valname ? arg->valname : "value");
    if (!STC_ARG_IS_BOOL(arg->type) && arg->def)
        fprintf(stream, " [default: %s]", (char *) arg->def);
    fputc('\n', stream);

    stc_arg_print_description(stream, arg->description);
}

static void stc_arg_print_description(FILE *stream, const char *description)
{
    size_t len;
    int    width;

    if (description) {
        len = strlen(description);
        while (*description) {
            while (isspace(*description)) description++;
            width = len < STC_ARG_DESC_MAX_WIDTH ? len : STC_ARG_DESC_MAX_WIDTH;
            while (description[width] && !isspace(description[width])) width--;
            fprintf(stream, "%*s%.*s\n", STC_ARG_DESC_PADDING, "", width,
                    description);
            description += width;
            len         -= width;
        }
    }
}

static int
stc_arg_process(const char *found, const StcArg *arg, const char *opt)
{
    int is_positional     = STC_ARG_IS_POSITIONAL(*arg);
    int has_eq            = found && *found == '=' && !is_positional;
    int consumed_next_arg = !is_positional && !has_eq;

    if (has_eq) found++;
    if (!STC_ARG_IS_BOOL(arg->type) && (!found || (has_eq && *found == '\0'))) {
        fprintf(stderr, "ERROR: missing value for argument '%s'\n", opt);
        return -1;
    }

    if (arg->out == NULL) {
        if (STC_ARG_IS_BOOL(arg->type) && has_eq) goto bool_with_eq;
        return consumed_next_arg;
    }

    switch (arg->type) {
        case STC_ARG_STR: *(const char **) arg->out = found; break;
        case STC_ARG_BOOL:
            if (has_eq) {
            bool_with_eq:
                fprintf(stderr,
                        "ERROR: unexpected value found for boolean flag '%s'\n",
                        opt);
                return -1;
            }
            consumed_next_arg = 0;
            *(int *) arg->out = arg->def == NULL;
            break;
        case STC_ARG_CUSTOM:
            switch (arg->convert(found, arg->out)) {
                case STC_ARG_CR_SUCCESS: break;
                case STC_ARG_CR_FAILURE:
                    fprintf(stderr,
                            "ERROR: invalid value '%s' for argument '%s'\n",
                            found, opt);
                case STC_ARG_CR_FAILURE_HANDLED: return -1;
            }
            break;
    }

    return consumed_next_arg;
}

static int stc_arg_memcpy(void          *dst,
                          const void    *src,
                          StcArgConvert *convert,
                          StcArgType     type)
{
    switch (type) {
        case STC_ARG_STR: *(const char **) dst = (const char *) src; break;
        case STC_ARG_BOOL: *(int *) dst = src != NULL; break;
        case STC_ARG_CUSTOM:
            if (convert((const char *) src, dst) != STC_ARG_CR_SUCCESS)
                return 0;
            break;
    }

    return 1;
}

static size_t is_prefixed(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? 0 : len;
}
