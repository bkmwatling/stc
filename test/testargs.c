#include <stdlib.h>
#include <string.h>

#include "../src/util/args.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define REALLY_LONG_DESCRIPTION                                                \
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque et" \
    " risus commodo, aliquet nulla vel, pretium leo. Praesent commodo "        \
    "venenatis leo ac interdum. Proin sodales cursus dui eu lobortis. Sed id " \
    "aliquam elit. Cras ipsum risus, sodales posuere neque scelerisque, "      \
    "feugiat tempor lorem. Integer eget est at est commodo semper pulvinar "   \
    "non eros. Mauris vel pulvinar turpis, sed commodo purus. In et augue "    \
    "quam. Suspendisse sed dolor odio. Nulla tempus varius neque, vel rutrum " \
    "odio. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas " \
    "sagittis quam augue, eget lacinia odio dapibus in. Aenean bibendum "      \
    "convallis imperdiet. Integer sed lobortis lacus."

typedef enum { STR, BOOL, INT } Type;

StcArgConvertResult convert_to_type(const char *arg, void *out)
{
    Type *type = out;

    if (strcmp(arg, "string") == 0)
        *type = STR;
    else if (strcmp(arg, "bool") == 0)
        *type = BOOL;
    else if (strcmp(arg, "int") == 0)
        *type = INT;
    else
        return STC_ARG_CR_FAILURE;

    return STC_ARG_CR_SUCCESS;
}

int main(int argc, const char *argv[])
{
    int           c, arg_idx;
    const char   *cmd, *name, *set, *filename, *database;
    Type          type;
    StcArgParser *argparser = stc_argparser_new(NULL);

    stc_argparser_add_argument(argparser, STC_ARG_STR, "<subcommand>", NULL,
                               &cmd, NULL, "Subcommand to execute", NULL, NULL);
    arg_idx = stc_argparser_parse(argparser, argc, argv);
    // argc    -= arg_idx - 1;
    // argv    += arg_idx - 1;

    stc_argparser_add_argument(argparser, STC_ARG_BOOL, "-c", "--create", &c,
                               NULL, "Creates the value if it does not exist",
                               NULL, NULL);
    stc_argparser_add_argument(argparser, STC_ARG_STR, "<name>", NULL, &name,
                               "name", "Name of value", NULL, NULL);
    stc_argparser_add_argument(
        argparser, STC_ARG_STR, NULL, "--set", &set, "set-string",
        "Sets the value to the given value (must exist)", NULL, NULL);
    stc_argparser_add_argument(
        argparser, STC_ARG_STR, "-o", "--output-file", &filename, "filename",
        "The name of the file to save the values to", "out.json", NULL);
    stc_argparser_add_argument(
        argparser, STC_ARG_CUSTOM, "-t", "--type", &type, "string | bool | int",
        "The type of the value for set", "bool", convert_to_type);
    stc_argparser_add_argument(argparser, STC_ARG_BOOL, NULL,
                               "--really-long-description", NULL, NULL,
                               REALLY_LONG_DESCRIPTION, NULL, NULL);
    stc_argparser_add_argument(argparser, STC_ARG_STR, NULL, "<database>",
                               &database, NULL, "Name of database of values",
                               "redis", NULL);
    stc_argparser_parse_exact(argparser, argc, argv);

    printf("cmd=%s\n", cmd);
    printf("c=%d\n", c);
    printf("name=%s\n", name);
    printf("set=%s\n", set);
    printf("filename=%s\n", filename);
    printf("type=%d\n", type);
    printf("database=%s\n", database);

    return EXIT_SUCCESS;
}
