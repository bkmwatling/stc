#include <stdio.h>
#include <stdlib.h>

#include "../vec/vec.h"

int main(int argc, char *argv[])
{
    int    i;
    size_t j;
    Vec    v     = vec_default(sizeof(char *)), w;
    char   vsa[] = "hellov", wsa[] = "whello";
    char  *vs = vsa, *ws = wsa;

    for (i = 1; i < argc; i++) { vec_append(&v, argv[i]); }
    w = vec_clone(v);

    printf("v:");
    for (j = 0; j < v.len; j++) { printf(" %s", VEC_AT(v, j, char *)); }

    printf("\nw:");
    for (j = 0; j < w.len; j++) { printf(" %s", VEC_AT(w, j, char *)); }
    printf("\nstuff\n");

    vec_append_literal(&v, vsa, char *);
    vec_append_literal(&w, wsa, char *);

    printf("v:");
    for (j = 0; j < v.len; j++) { printf(" %s", VEC_AT(v, j, char *)); }

    printf("\nw:");
    for (j = 0; j < w.len; j++) { printf(" %s", VEC_AT(w, j, char *)); }
    printf("\n");

    vec_free(v);
    vec_free(w);

    return EXIT_SUCCESS;
}
