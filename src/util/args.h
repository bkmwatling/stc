#ifndef STC_ARGS_H
#define STC_ARGS_H

typedef enum {
    STC_ARG_STR,  /**< string argument                                        */
    STC_ARG_BOOL, /**< boolean argument                                       */
} StcArgType;

typedef struct {
    StcArgType type;        /**< type of the argument                         */
    char      *shortopt;    /**< short option string (should start with "-")  */
    char      *longopt;     /**< long option string (should start with "--")  */
    char      *description; /**< optional description of the argument         */
    void      *def;         /**< optional default value of the argument       */
    void      *out;         /**< optional pointer to store argument value     */
} StcArg;

/**
 * Parses the command-line arguments using the given array of argument
 * specifications. This function reorders argv such that once it is done argv
 * has all the recognised arguments listed first (based off the given
 * specifications), then the unrecognised arguments after (still in their
 * relative order). Note that opts_len may be zero in which case opts is
 * expected to be null-terminated.
 *
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the array of command-line arguments
 * @param[in] opts the array of optional argument specifications
 * @param[in] opts_len the length the array of optional argument specifications
 * @param[in] usage the optional custom usage print function to override
 *                  printing usage based on given argument specifications
 *
 * @return the index into argv of the first unrecognised command-line argument
 *         after reordering argv to have recognised arguments first
 */
int stc_args_parse(int     argc,
                   char  **argv,
                   StcArg *opts,
                   int     opts_len,
                   void (*usage)(char *));

/**
 * This function is almost identical to stc_args_parse except if there are
 * unrecognised arguments, it prints the usage of the program and exits the
 * program with EXIT_FAILURE.
 */
void stc_args_parse_exact(int     argc,
                          char  **argv,
                          StcArg *opts,
                          int     opts_len,
                          void (*usage)(char *));

#endif /* STC_ARGS_H */
