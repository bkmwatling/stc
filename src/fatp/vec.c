#include <stdlib.h>
#include <string.h>

#include <stc/fatp/vec.h>

// TODO: use allocator instead of realloc
static void _stc_vec_resize(StcVec(void) *vec, size_t size, size_t cap)
{
    *vec              = (StcVec(void)) ((struct stc_vec_header *) realloc(
                               _stc_vec_header(*vec), size * cap) +
                           1);
    stc_vec_cap(*vec) = cap;
}

void _stc_vec_shift(StcVec(void) *vec,
                    size_t        idx_from,
                    size_t        idx_to,
                    size_t        size)
{
    memmove((char *) *vec + idx_to * size, (char *) *vec + idx_from * size,
            size * (stc_vec_len(*vec) - idx_from));
}

void _stc_vec_extend(StcVec(void) *vec, const void *p, size_t size, size_t len)
{
    if (len == 0) return;

    _stc_vec_reserve(vec, size, len);
    memcpy((char *) *vec + size * stc_vec_len(*vec), p, size * len);
    stc_vec_len(*vec) += len;
}

void _stc_vec_reserve_exact(StcVec(void) *vec, size_t size, size_t n)
{
    size_t len = stc_vec_len(*vec) + n;

    if (len > stc_vec_cap(*vec)) _stc_vec_resize(vec, size, len);
}

void _stc_vec_reserve(StcVec(void) *vec, size_t size, size_t n)
{
    size_t len = stc_vec_len(*vec) + n;
    size_t cap = stc_vec_cap(*vec) ? stc_vec_cap(*vec) : STC_VEC_DEFAULT_CAP;

    /* determine new cap by doubling it until enough space is reserved */
    while (len > cap) cap <<= 1;

    /* resize such that vec->cap becomes cap */
    if (cap > stc_vec_cap(*vec)) _stc_vec_resize(vec, size, cap);
}

void _stc_vec_shrink(StcVec(void) *vec, size_t size, size_t cap)
{
    if (cap < stc_vec_cap(*vec)) {
        if (cap < stc_vec_len(*vec)) cap = stc_vec_len(*vec);
        _stc_vec_resize(vec, size, cap);
    }
}
