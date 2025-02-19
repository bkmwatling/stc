#include <stdio.h>

#define STC_SLICE_ENABLE_SHORT_NAMES
#include <stc/fatp/slice.h>
#define STC_VEC_ENABLE_SHORT_NAMES
#include <stc/fatp/vec.h>

int main(int argc, char *argv[])
{
    int    i;
    size_t j;
    Vec(char *) v = vec_default(char *);
    Vec(char *) w;
    char vsa[] = "hellov", wsa[] = "whello";

    for (i = 1; i < argc; i++) vec_push_back(&v, argv[i]);
    w = vec_clone(v);
    printf("vec_pop_front(w): '%s'\n", vec_pop_front(&w));

    printf("v:");
    for (j = 0; j < vec_len(v); j++) printf(" '%s'", v[j]);

    printf("\nw:");
    for (j = 0; j < vec_len(w); j++) printf(" '%s'", w[j]);
    printf("\n\n");

    vec_push_back(&v, vsa);
    vec_push_back(&w, wsa);

    printf("v:");
    for (j = 0; j < vec_len(v); j++) printf(" '%s'", v[j]);

    printf("\nw:");
    for (j = 0; j < vec_len(w); j++) printf(" '%s'", w[j]);
    printf("\n");

    printf("\nAppending v to w.\n");
    vec_append(&w, &v);
    printf("v:");
    for (j = 0; j < vec_len(v); j++) printf(" '%s'", v[j]);
    printf("\nw:");
    for (j = 0; j < vec_len(w); j++) printf(" '%s'", w[j]);
    printf("\n");

    Slice(char *) s = vec_to_slice(w);
    printf("\ns:");
    for (j = 0; j < slice_len(s); j++) printf(" '%s'", s[j]);
    printf("\n");

    vec_free(v);
    vec_free(w);
    slice_free(s);

    return EXIT_SUCCESS;
}
