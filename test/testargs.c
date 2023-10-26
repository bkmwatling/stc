#include <stdlib.h>

#include "../src/util/args.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, const char *argv[])
{
    int         c;
    const char *name;
    const char *set;

    StcArg args[] = {
        {STC_ARG_BOOL, "-c",     "--create", NULL,       "Creates a value", NULL, &c   },
        { STC_ARG_STR, "<name>", NULL,       "name",     "Name of value",   NULL, &name},
        { STC_ARG_STR, NULL,     "--set",    "set-type", "Sets a value",    NULL, &set },
    };

    stc_args_parse_exact(argc, argv, args, ARR_LEN(args), NULL);
    printf("c=%d\n", c);
    printf("name=%s\n", name);
    printf("set=%s\n", set);

    return EXIT_SUCCESS;
}
