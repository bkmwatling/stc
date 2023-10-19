#include <stdlib.h>
#include <string.h>

#include "args.h"

#define STC_ARG_SHORTOPT_MATCHES(found, len, arg) \
    (arg->shortopt && (len = is_prefixed(found, arg->shortopt)) && !found[len])
#define STC_ARG_LONGOPT_MATCHES(found, len, arg)                  \
    ((arg->longopt && (len = is_prefixed(found, arg->longopt)) && \
      (found[len] == '=' || found[len] == '\0')))
#define STC_ARG_OPT_MATCHES(found, len, arg)      \
    (STC_ARG_SHORTOPT_MATCHES(found, len, arg) || \
     STC_ARG_LONGOPT_MATCHES(found, len, arg))

#define STC_ARGS_USAGE(usage, program, args, args_len) \
    (usage) ? usage(stderr, program)                   \
            : stc_args_usage(stderr, program, args, args_len)

/* --- Helper function prototypes ------------------------------------------- */

static int    stc_args_len(StcArg *args);
static void   stc_arg_usage(FILE *stream, StcArg *arg);
static int    stc_arg_process(const char   *found,
                              StcArg       *arg,
                              const char   *program,
                              StcArg       *args,
                              int           args_len,
                              StcArgsUsage *usage);
static void   stc_arg_memcpy(void *dst, const void *src, StcArgType type);
static size_t is_prefixed(const char *str, const char *prefix);

/* --- Public API function definitions -------------------------------------- */

int stc_args_parse(int           argc,
                   const char  **argv,
                   StcArg       *args,
                   int           args_len,
                   StcArgsUsage *usage)
{
    int         i, j, k, idx;
    size_t      len;
    int         npos = 0, nopt = 0, pos_idx = 0;
    int         arg_end = argc;
    int        *argset, *pos, *opts;
    const char *found;
    StcArg     *arg;
    void       *src;

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
            exit(EXIT_FAILURE);
        }

        if (STC_ARG_IS_POSITIONAL(args[i])) {
            npos++;
        } else {
            nopt++;
        }
    }

    /* populate the positional and optional argument indices arrays */
    pos  = malloc(npos * sizeof(int));
    opts = malloc(nopt * sizeof(int));
    for (i = j = k = 0; i < args_len; i++) {
        if (STC_ARG_IS_POSITIONAL(args[i])) {
            pos[j++] = i;
        } else {
            opts[k++] = i;
        }
    }

    /* loop through the command-line arguments and check for specified args */
    for (i = 1; i < arg_end; i++) {
        found = argv[i];

        if (found[0] == '-') {
            /* check for non-positional argument matches */
            for (j = 0; j < nopt; j++) {
                idx = opts[j];
                arg = args + idx;
                if (STC_ARG_OPT_MATCHES(found, len, arg)) {
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
        }

        /* process the command-line argument against specification */
        if (stc_arg_process(*found || i + 1 == arg_end ? found : argv[i + 1],
                            arg, argv[0], args, args_len, usage))
            i++;
        argset[idx] = 1;
    }

    /* check if any arguments were not found/set */
    for (i = k = 0; i < args_len; i++) {
        if (argset[i]) continue;
        arg = args + i;
        if (arg->def == NULL && !STC_ARG_IS_BOOL(arg->type)) {
            fprintf(stderr, "ERROR: Argument '%s' not specified\n",
                    arg->shortopt ? arg->shortopt : arg->longopt);
            STC_ARGS_USAGE(usage, argv[0], args, args_len);
            exit(EXIT_FAILURE);
        }
        if (arg->out) {
            src = STC_ARG_IS_BOOL(arg->type) ? &k : arg->def;
            stc_arg_memcpy(arg->out, src, arg->type);
        }
    }

    return arg_end;
}

void stc_args_parse_exact(int           argc,
                          const char  **argv,
                          StcArg       *args,
                          int           args_len,
                          StcArgsUsage *usage)
{
    int idx = stc_args_parse(argc, argv, args, args_len, usage);

    if (idx < argc) {
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

void stc_args_usage(FILE       *stream,
                    const char *program,
                    StcArg     *args,
                    int         args_len)
{
    int     i;
    StcArg *arg;

    /* print top usage line */
    fprintf(stream, "Usage: %s [OPTION]...", program);
    for (i = 0; i < args_len; i++) {
        if (!STC_ARG_IS_POSITIONAL(args[i])) continue;
        fprintf(stream, " %s",
                args[i].shortopt ? args[i].shortopt : args[i].longopt);
    }

    /* print descriptions of arguments */
    fprintf(stream, "\n\nList of positional arguments:\n");
    for (i = 0; i < args_len; i++) {
        arg = args + i;
        if (STC_ARG_IS_POSITIONAL(*arg)) stc_arg_usage(stream, arg);
    }

    fprintf(stream, "\nList of options:\n");
    for (i = 0; i < args_len; i++) {
        arg = args + i;
        if (!STC_ARG_IS_POSITIONAL(*arg)) stc_arg_usage(stream, arg);
    }
}

/* --- Helper function definitions ------------------------------------------ */

static int stc_args_len(StcArg *args)
{
    int    args_len = 0;
    StcArg arg_null = STC_ARG_NULL;

    for (; memcmp(&args[args_len], &arg_null, sizeof(StcArg)); args_len++)
        ;

    return args_len;
}

static void stc_arg_usage(FILE *stream, StcArg *arg)
{
    if (arg->shortopt) fprintf(stream, "  %s", arg->shortopt);
    if (arg->longopt) {
        if (arg->shortopt) {
            fprintf(stream, ", %s", arg->longopt);
        } else {
            fprintf(stream, "      %s", arg->longopt);
        }
    }

    /* TODO: print whether the argument requires a value */

    if (arg->description) fprintf(stream, "\t\t\t\t%s", arg->description);
    fprintf(stream, "\n");
}

static int stc_arg_process(const char   *found,
                           StcArg       *arg,
                           const char   *program,
                           StcArg       *args,
                           int           args_len,
                           StcArgsUsage *usage)
{
    int has_eq            = *found == '=';
    int consumed_next_arg = !STC_ARG_IS_POSITIONAL(*arg) && !has_eq;

    if (arg->out == NULL) {
        if (STC_ARG_IS_BOOL(arg->type) && has_eq) goto bool_with_eq;
        return consumed_next_arg;
    }
    if (has_eq) found++;

    switch (arg->type) {
        case STC_ARG_STR: *(const char **) arg->out = found; break;
        case STC_ARG_BOOL:
            if (has_eq) {
            bool_with_eq:
                fprintf(stderr,
                        "ERROR: unexpected value found for boolean flag\n");
                STC_ARGS_USAGE(usage, program, args, args_len);
                exit(EXIT_FAILURE);
            }
            consumed_next_arg = 0;
            *(int *) arg->out = 1;
            break;
    }

    return consumed_next_arg;
}

static void stc_arg_memcpy(void *dst, const void *src, StcArgType type)
{
    switch (type) {
        case STC_ARG_STR: *(char **) dst = *(char **) src; break;
        case STC_ARG_BOOL: *(int *) dst = *(int *) src; break;
    }
}

static size_t is_prefixed(const char *str, const char *prefix)
{
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) ? 0 : len;
}
