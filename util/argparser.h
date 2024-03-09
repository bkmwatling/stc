#ifndef STC_ARGPARSER_H
#define STC_ARGPARSER_H

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

typedef struct stc_argparser     StcArgParser;
typedef struct stc_subargparsers StcSubArgParsers;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_ARGS_ENABLE_SHORT_NAMES)
typedef StcArgParserUsage   ArgParserUsage;
typedef StcArgConvertResult ArgConvertResult;
typedef StcArgConvert       ArgConvert;
typedef StcArgType          ArgType;
typedef StcArgParser        ArgParser;
typedef StcSubArgParsers    SubArgParsers;

#    define ARG_CR_SUCCESS         STC_ARG_CR_SUCCESS
#    define ARG_CR_FAILURE         STC_ARG_CR_FAILURE
#    define ARG_CR_FAILURE_HANDLED STC_ARG_CR_FAILURE_HANDLED

#    define ARG_STR    STC_ARG_STR
#    define ARG_BOOL   STC_ARG_BOOL
#    define ARG_CUSTOM STC_ARG_CUSTOM

#    define argparser_new  stc_argparser_new
#    define argparser_free stc_argparser_free

#    define argparser_add_argument   stc_argparser_add_argument
#    define argparser_add_subparsers stc_argparser_add_subparsers
#    define argparser_parse          stc_argparser_parse
#    define argparser_parse_exact    stc_argparser_parse_exact
#    define argparser_print_usage    stc_argparser_print_usage
#    define argparser_check_for_help stc_argparser_check_for_help

#    define subargparsers_add_argparser stc_subargparsers_add_argparser
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
 * Add a string option specification for the argument parser to recognise in
 * command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] shortopt    short name string (must start with '-')
 * @param[in] longopt     long name string (must start with '--')
 * @param[in] valname     optional name of value to show in usage
 * @param[in] description optional description of the argument
 * @param[in] out         optional string pointer to store argument value
 * @param[in] def         optional string default value
 */
void stc_argparser_add_str_option(StcArgParser *self,
                                  const char   *shortopt,
                                  const char   *longopt,
                                  const char   *valname,
                                  const char   *description,
                                  const char  **out,
                                  const char   *def);

/**
 * Add a Boolean option specification for the argument parser to recognise in
 * command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] shortopt    short name string (must start with '-')
 * @param[in] longopt     long name string (must start with '--')
 * @param[in] description optional description of the argument
 * @param[in] out         optional string pointer to store argument value
 * @param[in] negate      whether to negate (invert) parsed argument
 *                        (i.e. if Boolean flag is found, set out to 0 instead
 *                        of truthy value, and if the flag is not found, set out
 *                        to truthy value instead of 0)
 */
void stc_argparser_add_bool_option(StcArgParser *self,
                                   const char   *shortopt,
                                   const char   *longopt,
                                   const char   *description,
                                   int          *out,
                                   int           negate);

/**
 * Add a custom type option specification for the argument parser to recognise
 * in command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] shortopt    short name string (must start with '-')
 * @param[in] longopt     long name string (must start with '--')
 * @param[in] valname     optional name of value to show in usage
 * @param[in] description optional description of the argument
 * @param[in] out         optional pointer to store argument value
 * @param[in] def         optional default value to pass to convert function
 * @param[in] convert     function to convert custom argument
 */
void stc_argparser_add_custom_option(StcArgParser  *self,
                                     const char    *shortopt,
                                     const char    *longopt,
                                     const char    *valname,
                                     const char    *description,
                                     void          *out,
                                     const char    *def,
                                     StcArgConvert *convert);

/**
 * Add a string positional argument specification for the argument parser to
 * recognise in command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] argname     name of positional argument to show in usage
 *                        (should not start with '-')
 * @param[in] description optional description of the argument
 * @param[in] out         optional string pointer to store argument value
 */
void stc_argparser_add_str_argument(StcArgParser *self,
                                    const char   *argname,
                                    const char   *description,
                                    const char  **out);

/**
 * Add a custom type positional argument specification for the argument parser
 * to recognise in command-line arguments.
 *
 * @param[in] self        the argument parser
 * @param[in] argname     name of positional argument to show in usage
 *                        (should not start with '-')
 * @param[in] description optional description of the argument
 * @param[in] out         optional pointer to store argument value
 * @param[in] convert     function to convert custom argument
 */
void stc_argparser_add_custom_argument(StcArgParser  *self,
                                       const char    *argname,
                                       const char    *description,
                                       void          *out,
                                       StcArgConvert *convert);

/**
 * Add a grouping of argument subparsers to the argument parser.
 *
 * @param[in] self   the argument parser to register subparser group to
 * @param[in] name   the name for the subparser group (for usage message)
 * @param[in] outcmd the optional pointer to store which subparser chosen
 * @param[in] outidx the optional pointer to store index of chosen subparser
 *
 * @return the subparser group to be able to add subparsers to
 */
StcSubArgParsers *stc_argparser_add_subparsers(StcArgParser *self,
                                               const char   *name,
                                               const char  **outcmd,
                                               size_t       *outidx);

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
void stc_argparser_parse(const StcArgParser *self, int argc, const char **argv);

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

/**
 * Add an argument subparser that can be parsed as one of the possible options
 * in the given argument subparsers.
 *
 * @param[in] self        the group of argument subparsers
 * @param[in] cmd         the name of the command to register this subparser as
 * @param[in] description the description of the subcommand
 * @param[in] usage       the optional custom usage print function for subparser
 *
 * @return the new argument subparser
 */
StcArgParser *stc_subargparsers_add_argparser(StcSubArgParsers  *self,
                                              const char        *cmd,
                                              const char        *description,
                                              StcArgParserUsage *usage);

#endif /* STC_ARGPARSER_H */
