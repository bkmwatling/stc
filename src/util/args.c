#include "args.h"

static void stc_args_usage(char *progam, StcArg *opts, int opts_len);

int stc_args_parse(int     argc,
                   char  **argv,
                   StcArg *opts,
                   int     opts_len,
                   void (*usage)(char *));

void stc_args_parse_exact(int     argc,
                          char  **argv,
                          StcArg *opts,
                          int     opts_len,
                          void (*usage)(char *));
