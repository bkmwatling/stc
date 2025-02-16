#include <stdlib.h>
#include <string.h>

#include <stc/fatp/vec.h>

// TODO: use allocator instead of malloc
static void _stc_vec_resize(StcVec(void) *vec, size_t size, size_t cap)
{
    vec->__stc_vec_data = realloc(vec->__stc_vec_data, size * cap);
    vec->cap            = cap;
}

void _stc_vec_shift(StcVec(void) *vec,
                    size_t        idx_from,
                    size_t        idx_to,
                    size_t        size)
{
    memmove((char *) vec->__stc_vec_data + idx_to * size,
            (char *) vec->__stc_vec_data + idx_from * size,
            size * (vec->len - idx_from));
}

void _stc_vec_extend(StcVec(void) *vec, const void *p, size_t size, size_t len)
{
    if (len == 0) return;

    _stc_vec_reserve(vec, size, len);
    memcpy((char *) vec->__stc_vec_data + size * vec->len, p, size * len);
    vec->len += len;
}

void _stc_vec_reserve_exact(StcVec(void) *vec, size_t size, size_t n)
{
    size_t len = vec->len + n, cap = vec->cap;

    if (len > cap) _stc_vec_resize(vec, size, len);
}

void _stc_vec_reserve(StcVec(void) *vec, size_t size, size_t n)
{
    size_t len = vec->len + n;
    size_t cap = vec->cap ? vec->cap : STC_VEC_DEFAULT_CAP;

    /* determine new cap by doubling it until enough space is reserved */
    while (len > cap) cap <<= 1;

    /* resize such that vec->cap becomes cap */
    if (cap > vec->cap) _stc_vec_resize(vec, size, cap);
}

void _stc_vec_shrink(StcVec(void) *vec, size_t size, size_t cap)
{
    if (cap < vec->cap) {
        if (cap < vec->len) cap = vec->len;
        _stc_vec_resize(vec, size, cap);
    }
}
