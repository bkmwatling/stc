#ifndef STC_ARGS_H
#define STC_ARGS_H

#include <stdio.h>

typedef void StcArgsUsage(FILE *stream, const char *program);

typedef enum {
    STC_ARG_STR,  /*<< string argument                                        */
    STC_ARG_BOOL, /*<< boolean argument                                       */
} StcArgType;

typedef struct {
    StcArgType type;        /*<< type of the argument                         */
    char      *shortopt;    /*<< short name string (must start with '-' if
                                 longopt does)                                */
    char      *longopt;     /*<< long name string (must start with '-' if
                                 shortopt does)                               */
    char      *valname;     /*<< optional name of the value to show in usage  */
    char      *description; /*<< optional description of the argument         */
    void      *def;         /*<< optional default value of the argument       */
    void      *out;         /*<< optional pointer to store argument value     */
} StcArg;

#define STC_ARG_NULL                          \
    {                                         \
        0, NULL, NULL, NULL, NULL, NULL, NULL \
    }

#define STC_ARG_IS_POSITIONAL(arg)                   \
    (((arg).shortopt && (arg).shortopt[0] != '-') || \
     ((arg).longopt && (arg).longopt[0] != '-'))
#define STC_ARG_IS_STR(argtype)  ((argtype) == STC_ARG_STR)
#define STC_ARG_IS_BOOL(argtype) ((argtype) == STC_ARG_BOOL)

/**
 * Parses the command-line arguments using the given array of argument
 * specifications. This function reorders argv such that once it is done argv
 * has all the recognised arguments listed first (based off the given
 * specifications), then the unrecognised arguments after (still in their
 * relative order). Note that opts_len may be zero in which case the last
 * element of opts is expected to be filled with zero and NULL values.
 *
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the array of command-line arguments
 * @param[in] args the array of argument specifications
 * @param[in] args_len the length the array of argument specifications
 * @param[in] usage the optional custom usage print function to override
 *                  printing usage based on given argument specifications
 *
 * @return the index into argv of the first unrecognised command-line argument
 *         after reordering argv to have recognised arguments first
 */
int stc_args_parse(int           argc,
                   const char  **argv,
                   const StcArg *args,
                   int           args_len,
                   StcArgsUsage *usage);

/**
 * This function is almost identical to stc_args_parse except if there are
 * unrecognised arguments, it prints the usage of the program and exits the
 * program with EXIT_FAILURE.
 */
void stc_args_parse_exact(int           argc,
                          const char  **argv,
                          const StcArg *args,
                          int           args_len,
                          StcArgsUsage *usage);

void stc_args_usage(FILE         *stream,
                    const char   *program,
                    const StcArg *args,
                    int           args_len);

#endif /* STC_ARGS_H */
