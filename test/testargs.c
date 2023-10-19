#include <stdlib.h>

#include "../src/util/args.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, const char *argv[])
{
    int    c;
    StcArg args[] = {
        {STC_ARG_BOOL, "-c",     "--create", "Creates a value", NULL, &c  },
        { STC_ARG_STR, "<name>", NULL,       "Name of value",   NULL, NULL},
        { STC_ARG_STR, NULL,     "--set",    "Sets a value",    NULL, NULL},
    };

    stc_args_parse_exact(argc, argv, args, ARR_LEN(args), NULL);

    return EXIT_SUCCESS;
}
