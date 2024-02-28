#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

/* --- Preprocessor directives ---------------------------------------------- */

#define STC_ARG_IS_POSITIONAL(arg)                     \
    (((arg)->shortopt && (arg)->shortopt[0] != '-') || \
     ((arg)->longopt && (arg)->longopt[0] != '-'))
#define STC_ARG_IS_STR(argtype)    ((argtype) == STC_ARG_STR)
#define STC_ARG_IS_BOOL(argtype)   ((argtype) == STC_ARG_BOOL)
#define STC_ARG_IS_CUSTOM(argtype) ((argtype) == STC_ARG_CUSTOM)

#define STC_ARG_DESC_PADDING   10
#define STC_ARG_DESC_MAX_WIDTH 70

#define STC_ARG_SHORTOPT_MATCHES(found, opt, len, arg) \
    ((opt = arg->shortopt) && (len = is_prefixed(found, opt)) && !found[len])
#define STC_ARG_LONGOPT_MATCHES(found, opt, len, arg)           \
    ((opt = arg->longopt) && (len = is_prefixed(found, opt)) && \
     (found[len] == '=' || found[len] == '\0'))
#define STC_ARG_OPT_MATCHES(found, opt, len, arg)      \
    (STC_ARG_SHORTOPT_MATCHES(found, opt, len, arg) || \
     STC_ARG_LONGOPT_MATCHES(found, opt, len, arg))

#define STC_ARGS_USAGE(argparser, stream, program)           \
    (argparser)->usage ? (argparser)->usage(stream, program) \
                       : stc_argparser_print_usage(argparser, program, stream)

/* --- Type definitions ----------------------------------------------------- */

typedef struct stc_arg StcArg;

struct stc_arg {
    StcArgType     type;        /**< type of the argument                     */
    int            idx;         /**< index into list of argument specifiers   */
    const char    *shortopt;    /**< short name string (must start with '-' if
                                     longopt does)                            */
    const char    *longopt;     /**< long name string (must start with '-' if
                                     shortopt does)                           */
    void          *out;         /**< optional pointer to store argument value */
    const char    *valname;     /**< optional name of value to show in usage  */
    const char    *description; /**< optional description of the argument     */
    const void    *def;         /**< optional default value of the argument
                                     (must be the string of the default value
                                     for the convert function for custom
                                     arguments)   */
    StcArgConvert *convert; /**< optional function to convert custom argument */

    StcArg *prev; /**< previous argument specification in list                */
    StcArg *next; /**< next argument specification in list                    */
};

struct stc_argparser {
    StcArgParserUsage *usage;
    StcArg            *sentinel;
};

/* --- Helper function prototypes ------------------------------------------- */

static void stc_arg_print_usage(FILE *stream, const StcArg *arg, int shortlen);
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
    StcArg *arg, *next;

    for (arg = self->sentinel->next; arg != self->sentinel; arg = next) {
        next = arg->next;
        free(arg);
    }

    free(self->sentinel);
    free(self);
}

void stc_argparser_add_argument(StcArgParser  *self,
                                StcArgType     argtype,
                                const char    *shortopt,
                                const char    *longopt,
                                void          *out,
                                const char    *valname,
                                const char    *description,
                                const void    *def,
                                StcArgConvert *convert)
{
    StcArg *arg = malloc(sizeof(*arg));

    arg->type        = argtype;
    arg->idx         = self->sentinel->idx++;
    arg->shortopt    = shortopt;
    arg->longopt     = longopt;
    arg->out         = out;
    arg->valname     = valname;
    arg->description = description;
    arg->def         = def;
    arg->convert     = convert;

    arg->prev       = self->sentinel->prev;
    arg->next       = self->sentinel;
    arg->prev->next = arg->next->prev = arg;
}

int stc_argparser_parse(const StcArgParser *self, int argc, const char **argv)
{
    int            i, j, k;
    size_t         len;
    int            npos = 0, nopt = 0, pos_idx = 0;
    int            arg_end = argc, done_opts = 0;
    int            exit_code;
    int           *argset;
    const StcArg **pos, **opts;
    const char    *found, *opt;
    const StcArg  *arg;

    /* determine the number of positional and optional arguments there are */
    for (arg = self->sentinel->next; arg != self->sentinel; arg = arg->next) {
        if (arg->shortopt && arg->longopt &&
            (arg->shortopt[0] == '-') != (arg->longopt[0] == '-')) {
            fprintf(stderr,
                    "ERROR: short and long names for argument must either "
                    "both reflect a non-positional argument (start with '-'), "
                    "or a positional argument (not start with '-'): "
                    "index=%d, short='%s', long='%s'",
                    arg->idx, arg->shortopt, arg->longopt);
            exit(EXIT_FAILURE);
        } else if (STC_ARG_IS_POSITIONAL(arg) && STC_ARG_IS_BOOL(arg->type)) {
            fprintf(stderr,
                    "ERROR: positional arguments cannot be Boolean: "
                    "argument='%s'\n",
                    arg->shortopt ? arg->shortopt : arg->longopt);
            exit(EXIT_FAILURE);
        }

        if (STC_ARG_IS_POSITIONAL(arg))
            npos++;
        else
            nopt++;
    }

    /* populate the positional and optional argument indices arrays */
    argset = calloc(self->sentinel->idx, sizeof(*argset));
    pos    = malloc(npos * sizeof(*pos));  // NOLINT(bugprone-sizeof-expression)
    opts   = malloc(nopt * sizeof(*opts)); // NOLINT(bugprone-sizeof-expression)
    for (arg = self->sentinel->next, i = j = k = 0; arg != self->sentinel;
         arg = arg->next, i++)
        if (STC_ARG_IS_POSITIONAL(arg))
            pos[j++] = arg;
        else
            opts[k++] = arg;

    /* loop through the command-line arguments and check for specified args */
    for (i = 1; i < arg_end; i++) {
        found = argv[i];

        if (found[0] == '-' && !done_opts) {
            /* check for '--' to indicate done with options */
            if (strcmp(found, "--") == 0) {
                done_opts = 1;
                goto unrecognised; /* not unrecognised, but to keep order */
            }

            /* check for non-positional argument matches */
            for (j = 0; j < nopt; j++) {
                arg = opts[j];
                if (STC_ARG_OPT_MATCHES(found, opt, len, arg)) {
                    found += len;
                    break;
                }
            }
            if (j >= nopt) goto unrecognised;
        } else {
            /* check if we still have positional arguments to match */
            if (pos_idx >= npos) {
            unrecognised:
                /* process unrecognised argument */
                memmove(argv + i, argv + i + 1,
                        (argc - i - 1) * sizeof(char *));
                argv[argc - 1] = found;
                arg_end--;
                i--;
                continue;
            }

            arg = pos[pos_idx++];
            opt = arg->shortopt ? arg->shortopt : arg->longopt;
        }

        /* process the command-line argument against specification */
        if (!STC_ARG_IS_POSITIONAL(arg) && *found == '\0')
            found = i + 1 < arg_end ? argv[i + 1] : NULL;
        if ((exit_code = stc_arg_process(found, arg, opt)) > 0) {
            i++;
        } else if (exit_code < 0) {
            exit_code = EXIT_FAILURE;
            goto exit_parsing;
        }
        argset[arg->idx] = 1;
    }

    /* check if any arguments were not found/set */
    for (arg = self->sentinel->next, i = 0; arg != self->sentinel;
         arg = arg->next, i++) {
        if (argset[i]) continue;
        if (arg->def == NULL && !STC_ARG_IS_BOOL(arg->type)) {
            stc_argparser_check_for_help(self, argc, argv, arg_end);
            fprintf(stderr, "ERROR: argument '%s' not specified\n",
                    arg->shortopt ? arg->shortopt : arg->longopt);
            exit_code = EXIT_FAILURE;
            goto exit_parsing;
        }
        if (arg->out &&
            !stc_arg_memcpy(arg->out, arg->def, arg->convert, arg->type)) {
            stc_argparser_check_for_help(self, argc, argv, arg_end);
            fprintf(stderr,
                    "ERROR: failed to set default value for argument '%s'\n",
                    arg->shortopt ? arg->shortopt : arg->longopt);
            exit_code = EXIT_FAILURE;
            goto exit_parsing;
        }
    }

    free(argset);
    free(pos);
    free(opts);
    return arg_end;

exit_parsing:
    free(argset);
    free(pos);
    free(opts);
    STC_ARGS_USAGE(self, exit_code == EXIT_SUCCESS ? stdout : stderr, argv[0]);
    exit(exit_code);
}

void stc_argparser_parse_exact(const StcArgParser *self,
                               int                 argc,
                               const char        **argv)
{
    int idx = stc_argparser_parse(self, argc, argv);

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
    int           len, has_arg, has_opt, pos_shortlen = 0, opt_shortlen = 0;
    const char   *shortopt;
    const StcArg *arg;

    /* check if there are any options or arguments */
    has_arg = has_opt = 0;
    for (arg = self->sentinel->next;
         arg != self->sentinel && (!has_arg || !has_opt); arg = arg->next)
        if (STC_ARG_IS_POSITIONAL(arg))
            has_arg = 1;
        else
            has_opt = 1;

    /* print top usage line */
    fprintf(stream, "Usage: %s%s", program, has_opt ? " [OPTIONS]" : "");
    for (arg = self->sentinel->next; arg != self->sentinel; arg = arg->next) {
        if (STC_ARG_IS_POSITIONAL(arg)) {
            shortopt = arg->shortopt ? arg->shortopt : arg->longopt;
            if (shortopt && (len = strlen(shortopt)) > pos_shortlen)
                pos_shortlen = len;
            fprintf(stream, " %s",
                    arg->shortopt ? arg->shortopt : arg->longopt);
        } else if (arg->shortopt &&
                   (len = strlen(arg->shortopt)) > opt_shortlen) {
            opt_shortlen = len;
        }
    }
    fprintf(stream, "\n");

    /* print descriptions of arguments */
    if (has_arg) {
        len = 0; /* use len as flag to indicate whether first argument done */
        fprintf(stream, "\nArguments:\n");
        for (arg = self->sentinel->next; arg != self->sentinel; arg = arg->next)
            if (STC_ARG_IS_POSITIONAL(arg)) {
                if (len)
                    fprintf(stream, "\n");
                else
                    len = 1;
                stc_arg_print_usage(stream, arg, pos_shortlen);
            }
    }

    /* print descriptions of options */
    if (has_opt) {
        len = 0; /* use len as flag to indicate whether first option done */
        fprintf(stream, "\nOptions:\n");
        for (arg = self->sentinel->next; arg != self->sentinel; arg = arg->next)
            if (!STC_ARG_IS_POSITIONAL(arg)) {
                if (len)
                    fprintf(stream, "\n");
                else
                    len = 1;
                stc_arg_print_usage(stream, arg, opt_shortlen);
            }
    }
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

/* --- Helper function definitions ------------------------------------------ */

static void stc_arg_print_usage(FILE *stream, const StcArg *arg, int shortlen)
{
    const char   *shortopt, *longopt, *description;
    unsigned long len;
    int           width = 2;

    shortopt = arg->shortopt ? arg->shortopt : "";
    longopt  = arg->longopt ? arg->longopt : "";
    if (STC_ARG_IS_POSITIONAL(arg) && arg->shortopt == NULL) {
        shortopt = longopt;
        longopt  = "";
        width    = 0;
    }
    fprintf(stream, "  %-*s%*s%s", shortlen, shortopt, width,
            arg->shortopt && arg->longopt ? ", " : "", longopt);

    if (!STC_ARG_IS_BOOL(arg->type) && !STC_ARG_IS_POSITIONAL(arg))
        fprintf(stream, " <%s>", arg->valname ? arg->valname : "value");
    if (!STC_ARG_IS_BOOL(arg->type) && arg->def)
        fprintf(stream, " [default: %s]", (char *) arg->def);
    fprintf(stream, "\n");

    if ((description = arg->description)) {
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
    int is_positional     = STC_ARG_IS_POSITIONAL(arg);
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
