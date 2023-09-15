#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define max(a, b) ((a) < (b) ? (b) : (a))

void *_vec_resize(void *vec, size_t size, size_t cap)
{
    void *w;

    w = realloc((vec) ? vec_header(vec) : NULL, size * cap + sizeof(VecHeader));
    w = (char *) w + sizeof(VecHeader);
    if (vec == NULL) { vec_header(w)->len = 0; }
    vec_header(w)->cap = cap;

    return w;
}

void *_vec_clone(const void *vec, size_t size)
{
    VecHeader *v;
    void      *clone;

    if (vec == NULL) { return NULL; }

    v     = vec_header(vec);
    clone = vec_new(size, v->len);
    memcpy(clone, vec, size * v->len);
    vec_header(clone)->len = v->len;

    return clone;
}

void *_vec_extend(void *vec, const void *slice, size_t size, size_t len)
{
    if (len == 0) { return vec; }
    vec = _vec_reserve(vec, size, len);

    if (vec == NULL) { return NULL; }

    memcpy((char *) vec + size * vec_len(vec), slice, size * len);
    vec_header(vec)->len += len;

    return vec;
}

void *_vec_reserve_exact(void *vec, size_t size, size_t n)
{
    size_t len = vec_len(vec) + n, cap = vec_cap(vec);

    return len > cap ? _vec_resize(vec, size, len) : vec;
}

void *_vec_reserve(void *vec, size_t size, size_t n)
{
    size_t len = vec_len(vec) + n, cap = vec ? vec_cap(vec) : 1;

    /* determine new cap by doubling it until enough space is reserved */
    while (len > cap) { cap <<= 1; }

    /* reserve exact amount of space such that vec->cap becomes cap */
    return _vec_reserve_exact(vec, size, cap - vec_len(vec));
}

void *_vec_shrink(void *vec, size_t size, size_t cap)
{
    return vec == NULL || cap < vec_cap(vec) && vec_len(vec) < vec_cap(vec)
               ? _vec_resize(vec, size, max(cap, vec_len(vec)))
               : vec;
}
