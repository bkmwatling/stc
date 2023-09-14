#include <stdlib.h>
#include <string.h>

#include "vec.h"

Vec vec_new(size_t size, size_t cap)
{
    return (Vec){ malloc(cap * size * sizeof(char)), size, 0, cap };
}

Vec vec_default(size_t size) { return vec_new(size, VEC_DEFAULT_CAP); }

Vec vec_clone(Vec vec)
{
    Vec clone = vec_new(vec.size, vec.len);
    _vec_extend(&clone, vec.data, vec.len);
    return clone;
}

int _vec_append(Vec *vec, const void *item)
{
    return _vec_extend(vec, item, 1);
}

int _vec_extend(Vec *vec, const void *slice, size_t len)
{
    if (len == 0) { return 1; }
    vec_reserve(vec, len);
    memcpy((char *) vec->data + vec->len * vec->size, slice, len * vec->size);
    vec->len += len;
    return 1;
}

void vec_reserve_exact(Vec *vec, size_t n)
{
    if (vec->len + n > vec->cap) {
        vec->cap  = vec->len + n;
        vec->data = realloc(vec->data, vec->cap * vec->size);
    }
}

void vec_reserve(Vec *vec, size_t n)
{
    size_t len = vec->len + n, cap = vec->cap;

    if (len > cap) {
        /* determine new cap by doubling it until enough space is reserved */
        do {
            cap <<= 1;
        } while (len > cap);

        /* reserve exact amount of space such that vec->cap becomes cap */
        vec_reserve_exact(vec, cap - vec->len);
    }
}
