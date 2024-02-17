#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

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

#define STC_ARGS_USAGE(stream, usage, program, args, args_len) \
    (usage) ? usage(stream, program)                           \
            : stc_args_usage(stream, program, args, args_len)

/* --- Helper function prototypes ------------------------------------------- */

static int  stc_args_len(const StcArg *args);
static void stc_arg_usage(FILE *stream, const StcArg *arg, int shortlen);
static int
stc_arg_process(const char *found, const StcArg *arg, const char *opt);
static int    stc_arg_memcpy(void          *dst,
                             const void    *src,
                             StcArgConvert *convert,
                             StcArgType     type);
static size_t is_prefixed(const char *str, const char *prefix);

/* --- Public function definitions ------------------------------------------ */

int stc_args_parse(int           argc,
                   const char  **argv,
                   const StcArg *args,
                   int           args_len,
                   StcArgsUsage *usage)
{
    int           i, j, k, idx;
    size_t        len;
    int           npos = 0, nopt = 0, pos_idx = 0;
    int           arg_end = argc, done_opts = 0;
    int           exit_code;
    int          *argset, *pos, *opts;
    const char   *found, *opt;
    const StcArg *arg;

    if (args_len <= 0) args_len = stc_args_len(args);
    argset = malloc(args_len * sizeof(int));
    memset(argset, 0, args_len * sizeof(int));

    /* determine the number of positional and optional arguments there are */
    for (i = 0; i < args_len; i++) {
        arg = args + i;
        if (arg->shortopt && arg->longopt &&
            (arg->shortopt[0] == '-') != (arg->longopt[0] == '-')) {
            fprintf(stderr,
                    "ERROR: short and long names for argument must either "
                    "both reflect a non-positional argument (start with '-'), "
                    "or a positional argument (not start with '-'): "
                    "index=%d, short='%s', long='%s'",
                    i, arg->shortopt, arg->longopt);
            free(argset);
            exit(EXIT_FAILURE);
        }

        if (STC_ARG_IS_POSITIONAL(args + i))
            npos++;
        else
            nopt++;
    }

    /* populate the positional and optional argument indices arrays */
    pos  = malloc(npos * sizeof(int));
    opts = malloc(nopt * sizeof(int));
    for (i = j = k = 0; i < args_len; i++)
        if (STC_ARG_IS_POSITIONAL(args + i))
            pos[j++] = i;
        else
            opts[k++] = i;

    /* loop through the command-line arguments and check for specified args */
    for (i = 1; i < arg_end; i++) {
        found = argv[i];

        fprintf(stderr, "done_opts = %d\n", done_opts);
        if (found[0] == '-' && !done_opts) {
            /* check for '--' to indicate done with options */
            if (strcmp(found, "--") == 0) {
                done_opts = 1;
                goto unrecognised; /* not unrecognised, but to keep order */
            }

            /* check for non-positional argument matches */
            for (j = 0; j < nopt; j++) {
                idx = opts[j];
                arg = args + idx;
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

            idx = pos[pos_idx++];
            arg = args + idx;
            opt = arg->shortopt ? arg->shortopt : arg->longopt;
        }

        /* process the command-line argument against specification */
        if (*found == '\0' && i + 1 < arg_end) found = argv[i + 1];
        if ((exit_code = stc_arg_process(found, arg, opt)) > 0) {
            i++;
        } else if (exit_code < 0) {
            exit_code = EXIT_FAILURE;
            goto exit_parsing;
        }
        argset[idx] = 1;
    }

    /* check if any arguments were not found/set */
    for (i = 0; i < args_len; i++) {
        if (argset[i]) continue;
        arg = args + i;
        if (arg->def == NULL && !STC_ARG_IS_BOOL(arg->type)) {
            stc_args_check_for_help(argc, argv, arg_end, args, args_len, usage);
            fprintf(stderr, "ERROR: argument '%s' not specified\n",
                    arg->shortopt ? arg->shortopt : arg->longopt);
            exit_code = EXIT_FAILURE;
            goto exit_parsing;
        }
        if (arg->out &&
            !stc_arg_memcpy(arg->out, arg->def, arg->convert, arg->type)) {
            stc_args_check_for_help(argc, argv, arg_end, args, args_len, usage);
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
    STC_ARGS_USAGE(exit_code == EXIT_SUCCESS ? stdout : stderr, usage, argv[0],
                   args, args_len);
    exit(exit_code);
}

void stc_args_parse_exact(int           argc,
                          const char  **argv,
                          const StcArg *args,
                          int           args_len,
                          StcArgsUsage *usage)
{
    int idx = stc_args_parse(argc, argv, args, args_len, usage);

    stc_args_check_for_help(argc, argv, idx, args, args_len, usage);
    if (idx < argc && (strcmp(argv[idx], "--") != 0 || ++idx < argc)) {
        fprintf(stderr, "ERROR: unrecognised argument '%s'\n", argv[idx]);
        if (usage) {
            usage(stderr, argv[0]);
        } else {
            if (args_len <= 0) args_len = stc_args_len(args);
            stc_args_usage(stderr, argv[0], args, args_len);
        }
        exit(EXIT_FAILURE);
    }
}

void stc_args_usage(FILE         *stream,
                    const char   *program,
                    const StcArg *args,
                    int           args_len)
{
    int           i, len, has_arg, has_opt, pos_shortlen = 0, opt_shortlen = 0;
    const char   *shortopt;
    const StcArg *arg;

    /* check if there are any options or arguments */
    has_arg = has_opt = 0;
    for (i = 0; i < args_len && (!has_arg || !has_opt); i++)
        if (STC_ARG_IS_POSITIONAL(args + i))
            has_arg = 1;
        else
            has_opt = 1;

    /* print top usage line */
    fprintf(stream, "Usage: %s%s", program, has_opt ? " [OPTIONS]" : "");
    for (i = 0; i < args_len; i++) {
        arg = args + i;
        if (STC_ARG_IS_POSITIONAL(args + i)) {
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
        for (i = 0; i < args_len; i++) {
            arg = args + i;
            if (STC_ARG_IS_POSITIONAL(arg)) {
                if (len)
                    fprintf(stream, "\n");
                else
                    len = 1;
                stc_arg_usage(stream, arg, pos_shortlen);
            }
        }
    }

    /* print descriptions of options */
    if (has_opt) {
        len = 0; /* use len as flag to indicate whether first option done */
        fprintf(stream, "\nOptions:\n");
        for (i = 0; i < args_len; i++) {
            arg = args + i;
            if (!STC_ARG_IS_POSITIONAL(arg)) {
                if (len)
                    fprintf(stream, "\n");
                else
                    len = 1;
                stc_arg_usage(stream, arg, opt_shortlen);
            }
        }
    }
}

void stc_args_check_for_help(int           argc,
                             const char  **argv,
                             int           arg_idx,
                             const StcArg *args,
                             int           args_len,
                             StcArgsUsage *usage)
{
    int i;

    for (i = arg_idx; i < argc && strcmp(argv[i], "--") != 0; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            if (usage) {
                usage(stdout, argv[0]);
            } else {
                if (args_len <= 0) args_len = stc_args_len(args);
                stc_args_usage(stdout, argv[0], args, args_len);
            }
            exit(EXIT_SUCCESS);
        }
    }
}

/* --- Helper function definitions ------------------------------------------ */

static int stc_args_len(const StcArg *args)
{
    int    args_len = 0;
    StcArg arg_null = STC_ARG_NULL;

    for (; memcmp(&args[args_len], &arg_null, sizeof(StcArg)); args_len++)
        ;

    return args_len;
}

static void stc_arg_usage(FILE *stream, const StcArg *arg, int shortlen)
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
    int has_eq            = *found == '=';
    int consumed_next_arg = !STC_ARG_IS_POSITIONAL(arg) && !has_eq;

    if (has_eq) found++;
    if (*found == '\0' && !STC_ARG_IS_BOOL(arg->type)) {
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
