#ifndef STC_ARGS_H
#define STC_ARGS_H

#include <stdio.h>

/**
 * Prototype for custom usage printing function.
 *
 * @param[in] stream  the I/O stream to print to
 * @param[in] program the name of the program as it is executed
 */
typedef void StcArgsUsage(FILE *stream, const char *program);

typedef enum {
    STC_ARG_CR_SUCCESS,         /*<< conversion was successful                */
    STC_ARG_CR_FAILURE,         /*<< conversion failed but not reported       */
    STC_ARG_CR_FAILURE_HANDLED, /*<< conversion failed but function reported  */
} StcArgConvertResult;

/**
 * Prototype for conversion function of an argument to a custom type. Note that
 * the string argument will never be NULL and thus does not have to be handled
 * since NULL default value means no default. The output pointer will never be
 * NULL and thus does not have to be checked.
 *
 * @param[in]  arg the string argument
 * @param[out] out the memory address where to store the custom converted value
 *
 * @return STC_CR_SUCCESS on successful converions, else STC_CR_FAILURE on
 *         failure, but function did not report it (in which case the parser
 *         will report it), else STC_CR_FAILURE_HANDLED on failure, but the
 *         function did report (thus the parser will not report anything but
 *         only print usage)
 */
typedef StcArgConvertResult StcArgConvert(const char *arg, void *out);

typedef enum {
    STC_ARG_STR,    /*<< string argument                                      */
    STC_ARG_BOOL,   /*<< boolean argument                                     */
    STC_ARG_CUSTOM, /*<< custom argument to be paired with convert function   */
} StcArgType;

typedef struct {
    StcArgType     type;        /*<< type of the argument                     */
    const char    *shortopt;    /*<< short name string (must start with '-' if
                                     longopt does)                            */
    const char    *longopt;     /*<< long name string (must start with '-' if
                                     shortopt does)                           */
    void          *out;         /*<< optional pointer to store argument value */
    const char    *valname;     /*<< optional name of value to show in usage  */
    const char    *description; /*<< optional description of the argument     */
    const void    *def;         /*<< optional default value of the argument
                                     (must be the string of the default value
                                     for the convert function for custom
                                     arguments)   */
    StcArgConvert *convert; /*<< optional function to convert custom argument */
} StcArg;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_ARGS_ENABLE_SHORT_NAMES)
typedef StcArgsUsage        ArgsUsage;
typedef StcArgConvertResult ArgConvertResult;
typedef StcArgConvert       ArgConvert;
typedef StcArgType          ArgType;
typedef StcArg              Arg;

#    define ARG_CR_SUCCESS         STC_ARG_CR_SUCCESS
#    define ARG_CR_FAILURE         STC_ARG_CR_FAILURE
#    define ARG_CR_FAILURE_HANDLED STC_ARG_CR_FAILURE_HANDLED

#    define ARG_STR    STC_ARG_STR
#    define ARG_BOOL   STC_ARG_BOOL
#    define ARG_CUSTOM STC_ARG_CUSTOM

#    define ARG_NULL          STC_ARG_NULL
#    define ARG_IS_POSITIONAL STC_ARG_IS_POSITIONAL
#    define ARG_IS_STR        STC_ARG_IS_STR
#    define ARG_IS_BOOL       STC_ARG_IS_BOOL
#    define ARG_IS_CUSTOM     STC_ARG_IS_CUSTOM

#    define args_parse       stc_args_parse
#    define args_parse_exact stc_args_parse_exact
#    define args_usage       stc_args_usage
#endif /* STC_ARGS_ENABLE_SHORT_NAMES */

#define STC_ARG_NULL                                \
    {                                               \
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL \
    }

#define STC_ARG_IS_POSITIONAL(arg)                     \
    (((arg)->shortopt && (arg)->shortopt[0] != '-') || \
     ((arg)->longopt && (arg)->longopt[0] != '-'))
#define STC_ARG_IS_STR(argtype)    ((argtype) == STC_ARG_STR)
#define STC_ARG_IS_BOOL(argtype)   ((argtype) == STC_ARG_BOOL)
#define STC_ARG_IS_CUSTOM(argtype) ((argtype) == STC_ARG_CUSTOM)

/**
 * Parses the command-line arguments using the given array of argument
 * specifications. This function reorders argv such that once it is done argv
 * has all the recognised arguments listed first (based off the given
 * specifications), then the unrecognised arguments after (still in their
 * relative order). Note that opts_len may be zero in which case the last
 * element of opts is expected to be filled with zero and NULL values.
 *
 * @param[in] argc     the number of command-line arguments
 * @param[in] argv     the array of command-line arguments
 * @param[in] args     the array of argument specifications
 * @param[in] args_len the length the array of argument specifications
 * @param[in] usage    the optional custom usage print function to override
 *                     printing usage based on given argument specifications
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

/**
 * Prints the usage information of the program based on the given argument
 * specifications.
 *
 * @param[in] stream   the I/O stream to print to
 * @param[in] program  the name of the program as it is executed
 * @param[in] args     the array of argument specifications
 * @param[in] args_len the length the array of argument specifications
 */
void stc_args_usage(FILE         *stream,
                    const char   *program,
                    const StcArg *args,
                    int           args_len);

/**
 * Checks for the '-h' or '--help' flags in the command-line arguments starting
 * from the given argument index `arg_idx` and prints the usage information to
 * stdout if found.
 *
 * @param[in] argc     the number of command-line arguments
 * @param[in] argv     the array of command-line arguments
 * @param[in] arg_idx  the start index of argv to search for the help flags
 * @param[in] args     the array of argument specifications
 * @param[in] args_len the length the array of argument specifications
 * @param[in] usage    the optional custom usage print function to override
 *                     printing usage based on given argument specifications
 */
void stc_args_check_for_help(int           argc,
                             const char  **argv,
                             int           arg_idx,
                             const StcArg *args,
                             int           args_len,
                             StcArgsUsage *usage);

#endif /* STC_ARGS_H */
