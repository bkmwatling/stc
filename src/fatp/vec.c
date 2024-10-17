#include <stdlib.h>
#include <string.h>

#include <stc/fatp/vec.h>

void stc_vec_free(void *vec)
{
    if (vec) free(stc_vec_header(vec));
}

static void *_stc_vec_resize(void *vec, size_t size, size_t cap)
{
    void *w;

    w = realloc(vec ? stc_vec_header(vec) : NULL,
                size * cap + sizeof(StcVecHeader));
    w = (StcVecHeader *) w + 1;
    if (vec == NULL) stc_vec_len_unsafe(w) = 0;
    stc_vec_cap_unsafe(w) = cap;

    return w;
}

void *_stc_vec_clone(const void *vec, size_t size)
{
    void *clone;

    if (stc_vec_len(vec) == 0) return NULL;

    clone = stc_vec_new(size, stc_vec_len_unsafe(vec));
    memcpy(clone, vec, size * stc_vec_len_unsafe(vec));
    stc_vec_len_unsafe(clone) = stc_vec_len_unsafe(vec);

    return clone;
}

void _stc_vec_shift(void *vec, size_t idx_from, size_t idx_to, size_t size)
{
    memmove((char *) vec + idx_to * size, (char *) vec + idx_from * size,
            size * (stc_vec_len_unsafe(vec) - idx_from));
}

void *_stc_vec_extend(void *vec, const void *p, size_t size, size_t len)
{
    if (len == 0) return vec;
    vec = _stc_vec_reserve(vec, size, len);

    if (vec == NULL) return NULL;

    memcpy((char *) vec + size * stc_vec_len_unsafe(vec), p, size * len);
    stc_vec_len_unsafe(vec) += len;

    return vec;
}

void *_stc_vec_reserve_exact(void *vec, size_t size, size_t n)
{
    size_t len = stc_vec_len(vec) + n, cap = stc_vec_cap(vec);

    return len > cap ? _stc_vec_resize(vec, size, len) : vec;
}

void *_stc_vec_reserve(void *vec, size_t size, size_t n)
{
    size_t len = stc_vec_len(vec) + n;
    size_t cap = vec ? stc_vec_cap_unsafe(vec) : STC_VEC_DEFAULT_CAP;

    /* determine new cap by doubling it until enough space is reserved */
    while (len > cap) cap <<= 1;

    /* resize such that vec->cap becomes cap */
    return cap > stc_vec_cap(vec) ? _stc_vec_resize(vec, size, cap) : vec;
}

void *_stc_vec_shrink(void *vec, size_t size, size_t cap)
{
    if (vec == NULL || (cap < stc_vec_cap_unsafe(vec) &&
                        stc_vec_len_unsafe(vec) < stc_vec_cap_unsafe(vec))) {
        if (cap < stc_vec_len(vec)) cap = stc_vec_len_unsafe(vec);
        vec = _stc_vec_resize(vec, size, cap);
    }

    return vec;
}
