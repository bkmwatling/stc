#ifndef STC_ARGS_H
#define STC_ARGS_H

#include <stdio.h>

/**
 * Prototype for custom usage printing function.
 *
 * @param[in] stream  the I/O stream to print to
 * @param[in] program the name of the program as it is executed
 */
typedef void StcArgParserUsage(FILE *stream, const char *program);

typedef enum {
    STC_ARG_CR_SUCCESS,         /**< conversion was successful                */
    STC_ARG_CR_FAILURE,         /**< conversion failed but not reported       */
    STC_ARG_CR_FAILURE_HANDLED, /**< conversion failed but function reported  */
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
    STC_ARG_STR,    /**< string argument                                      */
    STC_ARG_BOOL,   /**< boolean argument                                     */
    STC_ARG_CUSTOM, /**< custom argument to be paired with convert function   */
} StcArgType;

typedef struct stc_argparser StcArgParser;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_ARGS_ENABLE_SHORT_NAMES)
typedef StcArgParserUsage   ArgParserUsage;
typedef StcArgConvertResult ArgConvertResult;
typedef StcArgConvert       ArgConvert;
typedef StcArgType          ArgType;
typedef StcArgParser        ArgParser;

#    define ARG_CR_SUCCESS         STC_ARG_CR_SUCCESS
#    define ARG_CR_FAILURE         STC_ARG_CR_FAILURE
#    define ARG_CR_FAILURE_HANDLED STC_ARG_CR_FAILURE_HANDLED

#    define ARG_STR    STC_ARG_STR
#    define ARG_BOOL   STC_ARG_BOOL
#    define ARG_CUSTOM STC_ARG_CUSTOM

#    define argparser_new  stc_argparser_new
#    define argparser_free stc_argparser_free

#    define argparser_add_argument   stc_argparser_add_argument
#    define argparser_parse          stc_argparser_parse
#    define argparser_parse_exact    stc_argparser_parse_exact
#    define argparser_print_usage    stc_argparser_print_usage
#    define argparser_check_for_help stc_argparser_check_for_help
#endif /* STC_ARGS_ENABLE_SHORT_NAMES */

/**
 * Construct an argument parser with optional non-default usage print function.
 *
 * @param[in] usage optional usage print function
 *
 * @return the constructed argument parser
 */
StcArgParser *stc_argparser_new(StcArgParserUsage *usage);

/**
 * Free the memory allocated for an argument parser.
 *
 * @param[in] self the argument parser to free
 */
void stc_argparser_free(StcArgParser *self);

/**
 * Add an argument specification for the argument parser to recognise in
 * command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] argtype     type of the argument
 * @param[in] shortopt    short name string
 *                        (must start with '-' if longopt starts with '--')
 * @param[in] longopt     long name string
 *                        (must start with '--' if shortopt starts with '-')
 * @param[in] out         optional pointer to store argument value
 * @param[in] valname     optional name of value to show in usage
 * @param[in] description optional description of the argument
 * @param[in] def         optional default value of the argument
 *                        (must be the string of the default value for the
 *                        convert function for custom arguments)
 * @param[in] convert     optional function to convert custom argument
 */
void stc_argparser_add_argument(StcArgParser  *self,
                                StcArgType     argtype,
                                const char    *shortopt,
                                const char    *longopt,
                                void          *out,
                                const char    *valname,
                                const char    *description,
                                const void    *def,
                                StcArgConvert *convert);

/**
 * Parse the command-line arguments using the given argument parser. This
 * function reorders argv such that once it is done argv has all the recognised
 * arguments listed first (based off the given parser), then the unrecognised
 * arguments after (still in their relative order).
 *
 * @param[in] self the argument parser
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the array of command-line arguments
 *
 * @return the index into argv of the first unrecognised command-line argument
 *         after reordering argv to have recognised arguments first
 */
int stc_argparser_parse(const StcArgParser *self, int argc, const char **argv);

/**
 * This function is almost identical to stc_argparser_parse except if there are
 * unrecognised arguments, it prints the usage of the program and exits the
 * program with EXIT_FAILURE.
 *
 * @param[in] self the argument parser
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the array of command-line arguments
 */
void stc_argparser_parse_exact(const StcArgParser *self,
                               int                 argc,
                               const char        **argv);

/**
 * Print the usage information of the program based on the given argument
 * parser.
 *
 * @param[in] self    the argument parser
 * @param[in] program the name of the program as it is executed
 * @param[in] stream  the I/O stream to print to
 */
void stc_argparser_print_usage(const StcArgParser *self,
                               const char         *program,
                               FILE               *stream);

/**
 * Check for the '-h' or '--help' flags in the command-line arguments starting
 * from the given argument index `arg_idx` and print the usage information to
 * stdout if found.
 *
 * @param[in] self    the argument parser
 * @param[in] argc    the number of command-line arguments
 * @param[in] argv    the array of command-line arguments
 * @param[in] arg_idx the start index of argv to search for the help flags
 */
void stc_argparser_check_for_help(const StcArgParser *self,
                                  int                 argc,
                                  const char        **argv,
                                  int                 arg_idx);

#endif /* STC_ARGS_H */
