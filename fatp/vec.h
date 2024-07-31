#ifndef STC_VEC_H
#define STC_VEC_H

#include <stddef.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_VEC_ENABLE_SHORT_NAMES)
#    define Vec             StcVec
#    define VEC_DEFAULT_CAP STC_VEC_DEFAULT_CAP
#    define vec_header      stc_vec_header

#    define vec_new          stc_vec_new
#    define vec_init         stc_vec_init
#    define vec_default      stc_vec_default
#    define vec_default_init stc_vec_default_init
#    define vec_clone        stc_vec_clone
#    define vec_free         stc_vec_free

#    define vec_len        stc_vec_len
#    define vec_cap        stc_vec_cap
#    define vec_len_unsafe stc_vec_len_unsafe
#    define vec_cap_unsafe stc_vec_cap_unsafe
#    define vec_is_empty   stc_vec_is_empty
#    define vec_clear      stc_vec_clear

#    define vec_push_back  stc_vec_push_back
#    define vec_push_front stc_vec_push_front
#    define vec_pop        stc_vec_pop
#    define vec_first      stc_vec_first
#    define vec_last       stc_vec_last

#    define vec_insert   stc_vec_insert
#    define vec_remove   stc_vec_remove
#    define vec_drain    stc_vec_drain
#    define vec_truncate stc_vec_truncate

#    define vec_append            stc_vec_append
#    define vec_extend            stc_vec_extend
#    define vec_extend_from_slice stc_vec_extend_from_slice

#    define vec_reserve       stc_vec_reserve
#    define vec_reserve_exact stc_vec_reserve_exact
#    define vec_reserve_index stc_vec_reserve_index
#    define vec_shrink        stc_vec_shrink
#    define vec_shrink_to_fit stc_vec_shrink_to_fit

#    define vec_as_slice stc_vec_as_slice
#    define vec_to_slice stc_vec_to_slice
#endif /* STC_VEC_ENABLE_SHORT_NAMES */

typedef struct {
    size_t cap;
    size_t len;
} StcVecHeader;

/** Simple macro to show intention of using vector type. */
#define StcVec(T) T

#define STC_VEC_DEFAULT_CAP 4

#define stc_vec_header(v) (((StcVecHeader *) (v)) - 1)

#define stc_vec_new(size, cap)  _stc_vec_reserve_exact(NULL, (size), (cap))
#define stc_vec_init(v, cap)    ((v) = stc_vec_new(sizeof(*(v)), cap))
#define stc_vec_default(size)   stc_vec_new(size, STC_VEC_DEFAULT_CAP)
#define stc_vec_default_init(v) stc_vec_init(v, STC_VEC_DEFAULT_CAP)
#define stc_vec_clone(v)        _stc_vec_clone((v), sizeof(*(v)))

#define stc_vec_len(v)        ((v) ? stc_vec_len_unsafe(v) : 0)
#define stc_vec_cap(v)        ((v) ? stc_vec_cap_unsafe(v) : 0)
#define stc_vec_len_unsafe(v) (stc_vec_header(v)->len)
#define stc_vec_cap_unsafe(v) (stc_vec_header(v)->cap)
#define stc_vec_is_empty(v)   (stc_vec_len(v) == 0)
#define stc_vec_clear(v)      ((v) ? stc_vec_len_unsafe(v) = 0 : 0)

#define stc_vec_push_back(v, x) \
    (stc_vec_reserve(v, 1), (v)[stc_vec_len_unsafe(v)++] = (x))
#define stc_vec_push_front(v, x) stc_vec_insert(v, 0, x)
#define stc_vec_pop(v)           ((v)[--stc_vec_len_unsafe(v)])
#define stc_vec_first(v)         ((v)[0])
#define stc_vec_last(v)          ((v)[stc_vec_len_unsafe(v) - 1])

#define stc_vec_insert(v, i, x) \
    (stc_vec_reserve_index(v, i, 1), stc_vec_len_unsafe(v)++, (v)[i] = (x))
#define stc_vec_remove(v, i)      stc_vec_drain(v, i, 1)
#define stc_vec_swap_remove(v, i) ((v) ? ((v)[i] = stc_vec_pop(v), 1) : 0)
#define stc_vec_drain(v, i, n)                                 \
    ((v) ? (_stc_vec_shift((v), (i) + (n), (i), sizeof(*(v))), \
            stc_vec_len_unsafe(v) -= (n))                      \
         : 0)
#define stc_vec_truncate(v, len) \
    (len >= 0 && len < stc_vec_len(v) ? stc_vec_len_unsafe(v) = len : 0)

#define stc_vec_append(v, w) \
    (stc_vec_extend(v, w, stc_vec_len(w)), stc_vec_clear(w))
#define stc_vec_extend(v, p, len)          \
    (assert(sizeof(*(v)) == sizeof(*(p))), \
     (v) = _stc_vec_extend((v), (p), sizeof(*(p)), (len)))
#define stc_vec_extend_from_slice(v, s) stc_vec_extend(v, s, stc_slice_len(s))

#define stc_vec_reserve(v, n) ((v) = _stc_vec_reserve((v), sizeof(*(v)), (n)))
#define stc_vec_reserve_exact(v, n) \
    ((v) = _stc_vec_reserve_exact((v), sizeof(*(v)), (n)))
#define stc_vec_reserve_index(v, i, n) \
    (stc_vec_reserve(v, n), _stc_vec_shift((v), (i), (i) + (n), sizeof(*(v))))
#define stc_vec_shrink(v, cap) \
    ((v) = _stc_vec_shrink((v), sizeof(*(v)), (cap))
#define stc_vec_shrink_to_fit(v) stc_vec_shrink(v, stc_vec_len(v))

#define stc_vec_as_slice(v) (v)
#define stc_vec_to_slice(v) stc_slice_from_parts((v), stc_vec_len(v))

/**
 * Frees the memory allocated for a vector.
 *
 * @param[in] vec the pointer to the vector to free
 */
void stc_vec_free(void *vec);

/**
 * Clones (creates a copy of) a vector.
 *
 * @param[in] vec  the vector to clone
 * @param[in] size the size of each element
 *
 * @return a clone of the vector
 */
void *_stc_vec_clone(const void *vec, size_t size);

/**
 * Shifts the elements of a vector from one index to another.
 *
 * @param[in] vec      the pointer to the vector to shift
 * @param[in] idx_from the index to shift from in the vector
 * @param[in] idx_to   the index to shift to in the vector
 * @param[in] size     the size of each element
 */
void _stc_vec_shift(void *vec, size_t idx_from, size_t idx_to, size_t size);

/**
 * Extends the vector with the values from the memory pointed to by p.
 *
 * @param[in] vec  the pointer to the vector to extend
 * @param[in] p    the memory address of the data to extend from
 * @param[in] size the size of each element
 * @param[in] len  the length of (number of elements in) the data
 *
 * @return a pointer to the vector after extended with memory of p
 */
void *_stc_vec_extend(void *vec, const void *p, size_t size, size_t len);

/**
 * Reserves enough space in the vector for at least n more elements.
 *
 * @param[in] vec  the pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n    the number of elements to ensure space for
 *
 * @return a pointer to the vector after reserving the space
 */
void *_stc_vec_reserve(void *vec, size_t size, size_t n);

/**
 * Reserves exactly enough space in the vector for n more elements.
 *
 * @param[in] vec  the pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n    the number of elements to ensure space for
 *
 * @return a pointer to the vector after reserving the space
 */
void *_stc_vec_reserve_exact(void *vec, size_t size, size_t n);

/**
 * Shrinks the vector to have a capacity of exactly max(stc_vec_len(vec), cap)
 * if cap is less than the capacity of the vector.
 *
 * @param[in] vec  the pointer to the vector to shrink
 * @param[in] size the size of each element
 * @param[in] cap  the capacity to shrink to
 *
 * @return a pointer to the vector after shrinking
 */
void *_stc_vec_shrink(void *vec, size_t size, size_t cap);

#endif /* STC_VEC_H */
