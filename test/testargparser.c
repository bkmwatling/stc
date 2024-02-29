#include <stdlib.h>
#include <string.h>

#include "../src/util/argparser.h"

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
    const char       *cmd, *name, *filename, *database, *val;
    Type              type;
    StcArgParser     *argparser = stc_argparser_new(NULL);
    StcSubArgParsers *saps;
    StcArgParser     *create, *set;

    saps = stc_argparser_add_subparsers(argparser, "<subcmd>", &cmd);
    stc_argparser_add_str_option(argparser, "-o", "--output-file", "filename",
                                 "The name of the file to save the values to",
                                 &filename, "out.json");
    stc_argparser_add_bool_option(argparser, NULL, "--really-long-description",
                                  REALLY_LONG_DESCRIPTION, NULL, 0);
    stc_argparser_add_str_argument(argparser, "<database>",
                                   "Name of database of values", &database);

    create = stc_subargparsers_add_argparser(saps, "create",
                                             "Create a new value", NULL);
    stc_argparser_add_str_argument(create, "<name>", "Name of value", &name);
    stc_argparser_add_custom_option(
        create, "-t", "--type", "string | bool | int",
        "The type of the value to create", &type, "bool", convert_to_type);

    set = stc_subargparsers_add_argparser(
        saps, "set", "Sets the value to the given value (must exist)", NULL);
    stc_argparser_add_str_argument(set, "<name>", "Name of value", &name);
    stc_argparser_add_str_argument(set, "<value>", "Value to set name to",
                                   &val);

    stc_argparser_parse(argparser, argc, argv);

    printf("filename=%s\n", filename);
    if (strcmp(cmd, "create") == 0) {
        printf("name=%s\n", name);
        printf("type=%d\n", type);
    } else if (strcmp(cmd, "set") == 0) {
        printf("name=%s\n", name);
        printf("val=%s\n", val);
    }
    printf("database=%s\n", database);

    return EXIT_SUCCESS;
}
