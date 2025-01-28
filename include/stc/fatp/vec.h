#ifndef STC_VEC_H
#define STC_VEC_H

#include <stddef.h>

#include <stc/fatp/util.h>
#include <stc/util/macro.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_VEC_ENABLE_SHORT_NAMES)
#    define Vec             StcVec
#    define vec_define_for  stc_vec_define_for
#    define VEC_DEFAULT_CAP STC_VEC_DEFAULT_CAP

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
#    define vec_pop_back   stc_vec_pop_back
#    define vec_pop_front  stc_vec_pop_front
#    define vec_at         stc_vec_at
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

#define StcVec(T) STC_CONCAT(__stc_vec_, _STC_FATP_MAGIC_WORD_MACRO(T))

// TODO: add allocator field once allocator type is defined
#define stc_vec_define_for(T)                           \
    typedef struct {                                    \
        _STC_FATP_MAGIC_TYPE_MACRO(T) * __stc_vec_data; \
        size_t len, cap;                                \
    } StcVec(T)

/* NOTE: this is purely to indicate **any** type, and is an example of how to
 * define new types for StcVec. The definition allows the use of StcVec(void)
 * which means the elements of the vector are of type 'void' (which is obviously
 * useless, but serves well for casting to mean any type of elements).
 * See the bottom of this file for definitions of builtin types for StcVec. */
stc_vec_define_for(void);

#define _STC_VEC_ANY(v)     (*(StcVec(void) *) &(v))
#define _STC_VEC_PTR_ANY(v) ((StcVec(void) *) (v))

#define STC_VEC_DEFAULT_CAP 4

// TODO: use allocator instead of malloc and free
#define stc_vec_new(T, cap) \
    (StcVec(T)) { malloc(sizeof(_STC_FATP_MAGIC_TYPE_MACRO(T)) * cap), 0, cap }
#define stc_vec_init(v, cap) \
    _stc_vec_init(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), (cap))
#define stc_vec_default(T) stc_vec_new(T, STC_VEC_DEFAULT_CAP)

/**
 * Initialise the vector `v` with the default capacity.
 *
 * @param[in,out] v the vector to initialize
 */
#define stc_vec_default_init(v) stc_vec_init(v, STC_VEC_DEFAULT_CAP)
#define stc_vec_clone(v)              \
    (*_STC_FATP_AUTO_CAST_PTR_TO_PTR( \
        StcVec, StcVec, v,            \
        _stc_vec_clone(_STC_VEC_ANY(v), sizeof((v).__stc_vec_data))))
// *(StcVec(T) *) _stc_vec_clone(_STC_VEC_ANY(v), sizeof((v).__stc_vec_dta));
#define stc_vec_free(v) free((v).__stc_vec_data)

#define stc_vec_is_empty(v) ((v).len == 0)
#define stc_vec_clear(v)    ((v)->len = 0)

/**
 * Add the new element `x` to the tail of the vector `v`
 *
 * @param [in,out] v the vector to add the element to
 * @param [in]     x the element to add
 */
#define stc_vec_push_back(v, x)           \
    ({                                    \
        stc_vec_reserve(v, 1);            \
        stc_vec_at(*v, (v)->len++) = (x); \
    })
#define stc_vec_push_front(v, x) stc_vec_insert(v, 0, x)
#define stc_vec_pop_back(v)      stc_vec_at(*v, --(v)->len)
#define stc_vec_pop_front(v)                          \
    ({                                                \
        __auto_type _a = stc_vec_at(*v, 0);           \
        _stc_vec_shift(_STC_VEC_PTR_ANY(v), 1, 0,     \
                       sizeof(*(v)->__stc_vec_data)); \
        (v)->len--;                                   \
        _a;                                           \
    })
#define stc_vec_at(v, i) ((v).__stc_vec_data[i])
#define stc_vec_first(v) stc_vec_at(v, 0)

/**
 * Get the last element of the vector `v`.
 *
 * @param[in] v the vector
 *
 * @return the last element of the vector
 */
#define stc_vec_last(v) stc_vec_at(v, (v).len - 1)

#define stc_vec_insert(v, i, x) \
    (stc_vec_reserve_index(v, i, 1), (v)->len++, stc_vec_at(*v, i) = (x))
#define stc_vec_remove(v, i)      stc_vec_drain(v, i, 1)
#define stc_vec_swap_remove(v, i) (stc_vec_at(*v, i) = stc_vec_pop_back(v))
#define stc_vec_drain(v, i, n)                           \
    (_stc_vec_shift(_STC_VEC_PTR_ANY(v), (i) + (n), (i), \
                    sizeof(*(v)->__stc_vec_data)),       \
     (v)->len -= (n))
#define stc_vec_truncate(v, len) \
    (0 <= (len) && (len) < (v)->len ? ((v)->len = (len), 1) : 0)

#define stc_vec_append(v, w) (stc_vec_extend(v, w, (w)->len), stc_vec_clear(w))
#define stc_vec_extend(v, p, len)                          \
    (assert(sizeof(*(v)->__stc_vec_data) == sizeof(*(p))), \
     _stc_vec_extend(_STC_VEC_PTR_ANY(v), (p), sizeof(*(p)), (len)))
#define stc_vec_extend_from_slice(v, s) \
    stc_vec_extend(v, (s).__stc_slice_data, (s).len)

#define stc_vec_reserve(v, n) \
    (_stc_vec_reserve(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), (n)))
#define stc_vec_reserve_exact(v, n)                                            \
    (_stc_vec_reserve_exact(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), \
                            (n)))
#define stc_vec_reserve_index(v, i, n)                   \
    (stc_vec_reserve(v, n),                              \
     _stc_vec_shift(_STC_VEC_PTR_ANY(v), (i), (i) + (n), \
                    sizeof(*(v)->__stc_vec_data)))

#define stc_vec_shrink(v, cap)                                                \
    ((v) = _stc_vec_shrink(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), \
                           (cap)))

#define stc_vec_shrink_to_fit(v) stc_vec_shrink(v, (v)->len)

// TODO: figure out types
#define stc_vec_as_slice(v)          \
    _STC_FATP_AUTO_CAST_INITIALISER( \
        StcVec, StcSlice, v,         \
        { .__stc_slice_data = (v).__stc_vec_data, .len = (v).len })
#define stc_vec_to_slice(v) stc_slice_from_parts((v).__stc_vec_data, (v).len)

/**
 * Initialise a vector with a given capacity and allocator.
 *
 * @param[in,out] vec       the vector to initialise
 * @param[in]     size      the size of a single element in the vector
 * @param[in]     cap       the capacity to the initialise the vector with
 */
void _stc_vec_init(StcVec(void) *vec, size_t size, size_t cap);

/**
 * Clone (create a copy of) a vector.
 *
 * @param[in] vec  the vector to clone
 * @param[in] size the size of each element
 *
 * @return a clone of the vector
 */
StcVec(void) *_stc_vec_clone(StcVec(void) vec, size_t size);

/**
 * Shift the elements of a vector from one index to another.
 *
 * @param[in] vec      the pointer to the vector to shift
 * @param[in] idx_from the index to shift from in the vector
 * @param[in] idx_to   the index to shift to in the vector
 * @param[in] size     the size of each element
 */
void _stc_vec_shift(StcVec(void) *vec,
                    size_t        idx_from,
                    size_t        idx_to,
                    size_t        size);

/**
 * Extend the vector with the values from the memory pointed to by p.
 *
 * @param[in] vec  the pointer to the vector to extend
 * @param[in] p    the memory address of the data to extend from
 * @param[in] size the size of each element
 * @param[in] len  the length of (number of elements in) the data
 */
void _stc_vec_extend(StcVec(void) *vec, const void *p, size_t size, size_t len);

/**
 * Reserve enough space in the vector for at least n more elements.
 *
 * @param[in] vec  the pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n    the number of elements to ensure space for
 */
void _stc_vec_reserve(StcVec(void) *vec, size_t size, size_t n);

/**
 * Reserve exactly enough space in the vector for n more elements.
 *
 * @param[in] vec  the pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n    the number of elements to ensure space for
 */
void _stc_vec_reserve_exact(StcVec(void) *vec, size_t size, size_t n);

/**
 * Shrink the vector to have a capacity of exactly max(vec->len, cap)
 * if cap is less than the capacity of the vector.
 *
 * @param[in] vec  the pointer to the vector to shrink
 * @param[in] size the size of each element
 * @param[in] cap  the capacity to shrink to
 */
void _stc_vec_shrink(StcVec(void) *vec, size_t size, size_t cap);

/* --- Define StcVec for builtin types -------------------------------------- */

_STC_FATP_DEFINE_FOR_BUILTIN_TYPES(StcVec, stc_vec_define_for);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    include <stdint.h>
_STC_FATP_DEFINE_FOR_STDC99_TYPES(StcVec, stc_vec_define_for);
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

#if defined(STC_COMMON_H) && !defined(STC_DISABLE_BASIC_TYPES)
#    if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
_STC_FATP_ALIAS_TYPES_COMMON_FROM_STDC99(StcVec);
#    else
_STC_FATP_ALIAS_TYPES_COMMON_FROM_BUILTIN(StcVec);
#    endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */
#endif     /* defined(STC_COMMON_H) && !defined(STC_DISABLE_BASIC_TYPES) */

/* --- End definition of StcVec for builtin types --------------------------- */

#endif /* STC_VEC_H */
