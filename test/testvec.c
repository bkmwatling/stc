#include <stdio.h>
#include <stdlib.h>

#define STC_VEC_ENABLE_SHORT_NAMES
#include <stc/fatp/vec.h>

int main(int argc, char *argv[])
{
    int    i;
    size_t j;
    Vec(char ptr) v = vec_default(char ptr);
    Vec(char ptr) w;
    char vsa[] = "hellov", wsa[] = "whello";

    for (i = 1; i < argc; i++) vec_push_back(&v, argv[i]);
    w = vec_clone(v);
    printf("vec_pop_front(w): '%s'\n", vec_pop_front(&w));

    printf("v:");
    for (j = 0; j < v.len; j++) printf(" '%s'", vec_at(v, j));

    printf("\nw:");
    for (j = 0; j < w.len; j++) printf(" '%s'", vec_at(w, j));
    printf("\nstuff\n");

    vec_push_back(&v, vsa);
    vec_push_back(&w, wsa);

    printf("v:");
    for (j = 0; j < v.len; j++) printf(" '%s'", vec_at(v, j));

    printf("\nw:");
    for (j = 0; j < w.len; j++) printf(" '%s'", vec_at(w, j));
    printf("\n");

    vec_free(v);
    vec_free(w);

    return EXIT_SUCCESS;
}
