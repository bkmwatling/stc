#include <stdio.h>
#include <stdlib.h>

#define STC_VEC_ENABLE_SHORT_NAMES
#include "../src/fatp/vec.h"

int main(int argc, char *argv[])
{
    int    i;
    size_t j;
    char **v     = vec_default(sizeof(char *)), **w;
    char   vsa[] = "hellov", wsa[] = "whello";

    for (i = 1; i < argc; i++) { vec_push_back(v, argv[i]); }
    w = vec_clone(v);

    printf("v:");
    for (j = 0; j < vec_len(v); j++) { printf(" %s", v[j]); }

    printf("\nw:");
    for (j = 0; j < vec_len(w); j++) { printf(" %s", w[j]); }
    printf("\nstuff\n");

    vec_push_back(v, vsa);
    vec_push_back(w, wsa);

    printf("v:");
    for (j = 0; j < vec_len(v); j++) { printf(" %s", v[j]); }

    printf("\nw:");
    for (j = 0; j < vec_len(w); j++) { printf(" %s", w[j]); }
    printf("\n");

    vec_free(v);
    vec_free(w);

    return EXIT_SUCCESS;
}
