#ifndef STC_VEC_H
#define STC_VEC_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stc/fatp/slice.h>
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

#    define vec_is_empty stc_vec_is_empty
#    define vec_clear    stc_vec_clear

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

/**
 * Macro to use a vector with specified element type.
 *
 * NOTE: StcVec(void) can be used to indicate a vector of unknown/any type.
 * NOTE: Due to C's limited macro system, pointers are specified with the word
 *       `ptr` instead of the * symbol.
 *
 * @param[in] T the type of the vector elements
 *
 * @return the vector type specifier for the element type
 */
#define StcVec(T) STC_CONCAT(__stc_vec_, _STC_FATP_MAGIC_WORD_MACRO(T))

// TODO: add allocator field once allocator type is defined
/**
 * Macro to define a vector with underlying element type.
 *
 * NOTE: Vectors have been defined for all primitive types (including C99
 *       stdint.h), but must be defined for any other types you wish to use.
 * NOTE: Due to C's limited macro system, pointers are specified with the word
 *       `ptr` instead of the * symbol.
 *
 * @param[in] T the type of the vector elements
 */
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
/**
 * Create a new vector with given type and capacity, allocating the necessary
 * underlying memory and setting the length to 0.
 *
 * @param[in] T   the type of the vector
 * @param[in] cap the capacity of the vector to create
 *
 * @return the created vector
 */
#define stc_vec_new(T, cap) \
    (StcVec(T)) { malloc(sizeof(_STC_FATP_MAGIC_TYPE_MACRO(T)) * cap), 0, cap }

/**
 * Initialise a given vector's memory to a given capacity, setting the length to
 * 0.
 *
 * @param[in,out] v   the vector to initialise
 * @param[in]     cap the capacity of the vector to initialise
 */
#define stc_vec_init(v, cap) \
    _stc_vec_init(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), (cap))

/**
 * Create a new vector with given type and default capacity, allocating the
 * necessary underlying memory and setting the length to 0.
 *
 * @param[in] T the type of the vector
 *
 * @return the created vector with default capacity
 */
#define stc_vec_default(T) stc_vec_new(T, STC_VEC_DEFAULT_CAP)

/**
 * Initialise a given vector's memory to the default capacity, setting the
 * length to 0.
 *
 * @param[in,out] v the vector to initialise with default capacity
 */
#define stc_vec_default_init(v) stc_vec_init(v, STC_VEC_DEFAULT_CAP)

/**
 * Create a (shallow) clone of a vector by copying the underlying data.
 *
 * @param[in] v the vector to clone
 *
 * @return the clone of the vector
 */
#define stc_vec_clone(v)                                                    \
    ({                                                                      \
        __typeof__(v) _STC_MACRO_VAR(_v_) = {                               \
            malloc(sizeof(*(v).__stc_vec_data) * (v).len), (v).len, (v).len \
        };                                                                  \
        if ((v).len)                                                        \
            memcpy(_STC_MACRO_VAR(_v_).__stc_vec_data, (v).__stc_vec_data,  \
                   sizeof(*(v).__stc_vec_data) * (v).len);                  \
        _STC_MACRO_VAR(_v_);                                                \
    })

/**
 * Free the underlying memory of a vector.
 *
 * @param[in] v the vector to free the underlying memory of
 */
#define stc_vec_free(v) free((v).__stc_vec_data)

/**
 * Check whether a vector is empty (has a length of 0).
 *
 * @param[in] v the vector to check if empty
 *
 * @return a truthy value if the vector is empty; else 0
 */
#define stc_vec_is_empty(v) ((v).len == 0)

/**
 * Clear a vector to have a length of 0.
 *
 * NOTE: The underlying elements are not freed by this macro. Cleaning up memory
 *       is left to the caller.
 *
 * @param[in,out] v the vector to clear
 */
#define stc_vec_clear(v) ((v)->len = 0)

/**
 * Add an element to the tail of a vector.
 *
 * @param[in,out] v the vector to add the element to
 * @param[in]     x the element to add to the vector
 */
#define stc_vec_push_back(v, x)           \
    ({                                    \
        stc_vec_reserve(v, 1);            \
        stc_vec_at(*v, (v)->len++) = (x); \
    })

/**
 * Add an element to the head of a vector.
 *
 * @param[in,out] v the vector to add the element to
 * @param[in]     x the element to add to the vector
 */
#define stc_vec_push_front(v, x) stc_vec_insert(v, 0, x)

/**
 * Remove and return the element at the tail of a vector.
 *
 * @param[in,out] v the vector to pop the tail element from
 *
 * @return the tail element of the vector
 */
#define stc_vec_pop_back(v) stc_vec_at(*v, --(v)->len)

/**
 * Remove and return the element at the head of a vector.
 *
 * @param[in,out] v the vector to pop the head element from
 *
 * @return the head element of the vector
 */
#define stc_vec_pop_front(v)                                                   \
    ({                                                                         \
        __typeof__(stc_vec_at(*v, 0)) _STC_MACRO_VAR(_a_) = stc_vec_at(*v, 0); \
        _stc_vec_shift(_STC_VEC_PTR_ANY(v), 1, 0,                              \
                       sizeof(*(v)->__stc_vec_data));                          \
        (v)->len--;                                                            \
        _STC_MACRO_VAR(_a_);                                                   \
    })

/**
 * Get the element of a vector at a specified index.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] v the vector to get the indexed element from
 * @param[in] i the index of the element to retrieve
 *
 * @return the indexed element from the vector
 */
#define stc_vec_at(v, i) ((v).__stc_vec_data[i])

/**
 * Get the first element from a vector.
 *
 * @param[in] v the vector to retrieve the first element of
 *
 * @return the first element of the vector
 */
#define stc_vec_first(v) stc_vec_at(v, 0)

/**
 * Get the last element from a vector.
 *
 * @param[in] v the vector to retrieve the last element of
 *
 * @return the last element of the vector
 */
#define stc_vec_last(v) stc_vec_at(v, (v).len - 1)

/**
 * Insert an element into a vector at the specified index.
 *
 * @param[in,out] v the vector to insert the element into
 * @param[in]     i the index where to insert the element
 * @param[in]     x the element to insert into the vector
 */
#define stc_vec_insert(v, i, x) \
    (stc_vec_reserve_index(v, i, 1), (v)->len++, stc_vec_at(*v, i) = (x))

/**
 * Remove and return the element at the specified index from a vector.
 *
 * @param[in,out] v the vector to remove the element from
 * @param[in]     i the index of the element to remove
 *
 * @return the element removed from the vector
 */
#define stc_vec_remove(v, i)                                                   \
    ({                                                                         \
        __typeof__(stc_vec_at(*v, i)) _STC_MACRO_VAR(_x_) = stc_vec_at(*v, i); \
        stc_vec_drain(v, i, 1);                                                \
        _STC_MACRO_VAR(_x_);                                                   \
    })

/**
 * Remove and return the element at the specified index from a vector, by
 * swapping it with the last element of the vector for efficiency.
 *
 * This macro is useful when the order of the elements in the vector don't
 * matter, as it is far more efficient to swap a single element than it is to
 * shift the entire underlying memory.
 *
 * @param[in,out] v the vector to remove the element from
 * @param[in]     i the index of the element to remove
 *
 * @return the element removed from the vector
 */
#define stc_vec_swap_remove(v, i)                                                \
    ({                                                                           \
        __typeof__(stc_vec_at(*v, i)) _STC_MACRO_VAR(_x_) = stc_vec_at(*v, i);   \
        stc_vec_at(*v, i)                                 = stc_vec_pop_back(v); \
        _STC_MACRO_VAR(_x_);                                                     \
    })

/**
 * Drain (remove) the specified number of elements starting from a specified
 * index from a vector.
 *
 * NOTE: The underlying drained elements are not freed by this macro. Cleaning
 *       up memory is left to the caller.
 *
 * @param[in,out] v the vector to drain the elements from
 * @param[in]     i the index to drain the elements from
 * @param[in]     n the number of elements to drain
 */
#define stc_vec_drain(v, i, n)                           \
    (_stc_vec_shift(_STC_VEC_PTR_ANY(v), (i) + (n), (i), \
                    sizeof(*(v)->__stc_vec_data)),       \
     (v)->len -= (n))

/**
 * Truncate the vector to the specified length, removing the elements between
 * the specified length and the length of the vector.
 *
 * NOTE: The vector is only truncated if the specified length is between 0 and
 *       the length of the vector. The only case where 0 is returned by this
 *       macro is when this condition is not met.
 * NOTE: The underlying removed elements are not freed by this macro. Cleaning
 *       up memory is left to the caller.
 *
 * @param[in,out] v   the vector to truncate
 * @param[in]     len the length to truncate the vector to
 *
 * @return a truthy value if the vector was truncated; else 0
 */
#define stc_vec_truncate(v, len) \
    (0 <= (len) && (len) < (v)->len ? ((v)->len = (len), 1) : 0)

/**
 * Move all the elements from a vector to the end of another vector.
 *
 * NOTE: After the append, the second vector will be empty.
 *
 * @param[in,out] v the vector to move elements to the end of
 * @param[in,out] w the vector to move the elements from
 */
#define stc_vec_append(v, w) \
    (stc_vec_extend(v, (w)->__stc_vec_data, (w)->len), stc_vec_clear(w))

/**
 * Extend a vector (append to its tail) with the elements from an array with
 * specified length.
 *
 * @param[in,out] v   the vector to extend
 * @param[in]     p   the array of elements to add to the vector
 * @param[in]     len the length of the array being extended from
 */
#define stc_vec_extend(v, p, len)                                       \
    ({                                                                  \
        static_assert(sizeof(*(v)->__stc_vec_data) == sizeof(*(p)),     \
                      "invalid size of array elements for StcVec");     \
        _stc_vec_extend(_STC_VEC_PTR_ANY(v), (p), sizeof(*(p)), (len)); \
    })

/**
 * Extend a vector (append to its tail) with the elements from a slice.
 *
 * @param[in,out] v the vector to extend
 * @param[in]     s the slice of elements to add to the vector
 */
#define stc_vec_extend_from_slice(v, s) \
    stc_vec_extend(v, (s).__stc_slice_data, (s).len)

/**
 * Reserve enough space in a vector for the specified number of elements.
 *
 * NOTE: More space than may be allocated than may be necessary.
 *
 * @param[in,out] v the vector to reserve space for
 * @param[in]     n the number of elements to reserve space for in the vector
 */
#define stc_vec_reserve(v, n) \
    (_stc_vec_reserve(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), (n)))

/**
 * Reserve exactly enough space in a vector for the specified number of
 * elements.
 *
 * NOTE: The exact amount of space necessary is allocated.
 *
 * @param[in,out] v the vector to reserve exact space for
 * @param[in]     n the number of elements to reserve exact space for in the
 *                  vector
 */
#define stc_vec_reserve_exact(v, n)                                            \
    (_stc_vec_reserve_exact(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), \
                            (n)))

/**
 * Reserve enough space in a vector for the specified number of elements at the
 * specified index.
 *
 * The space is allocated at the specified index, essentially leaving a gap in
 * the vector at that index.
 * NOTE: More space than may be allocated than may be necessary.
 *
 * @param[in,out] v the vector to reserve space for
 * @param[in]     i the index to reserve space at
 * @param[in]     n the number of elements to reserve space for in the vector
 */
#define stc_vec_reserve_index(v, i, n)                   \
    (stc_vec_reserve(v, n),                              \
     _stc_vec_shift(_STC_VEC_PTR_ANY(v), (i), (i) + (n), \
                    sizeof(*(v)->__stc_vec_data)))

/**
 * Shrink a vector to the specified capacity.
 *
 * NOTE: The vector will only shrink to its length at minimum, and will only
 *       shrink if the specified capacity is less than the vector's capacity.
 *
 * @param[in,out] v   the vector to shrink
 * @param[in]     cap the capacity to shrink the vector to
 */
#define stc_vec_shrink(v, cap) \
    (_stc_vec_shrink(_STC_VEC_PTR_ANY(v), sizeof(*(v)->__stc_vec_data), (cap)))

/**
 * Shrink a vector so that it's capacity and length are the same.
 *
 * @param[in,out] v the vector to shrink to its length
 */
#define stc_vec_shrink_to_fit(v) stc_vec_shrink(v, (v)->len)

/**
 * Create a slice that REFERENCES the data of a vector.
 *
 * NOTE: The created slice points to the vector's underlying data, and does NOT
 *       create a copy of the data, and thus must NOT be freed.
 *
 * @param[in] v the vector to create a slice reference of
 *
 * @return the slice which references the data of the vector
 */
#define stc_vec_as_slice(v)                                                   \
    ({                                                                        \
        StcSlice(void) _STC_MACRO_VAR(_s_) = { (v).__stc_vec_data, (v).len }; \
        _STC_FATP_AUTO_CAST_VAL_TO_VAL(StcVec, StcSlice, v,                   \
                                       _STC_MACRO_VAR(_s_));                  \
    })

/**
 * Create a slice that has a copy of the data of a vector.
 *
 * NOTE: The created slice points to a copy of the vector's underlying data, and
 *       thus MUST be freed.
 *
 * @param[in] v the vector to create a slice copy of
 *
 * @return the slice which has a copy of the data of the vector
 */
#define stc_vec_to_slice(v)                                                    \
    ({                                                                         \
        StcSlice(void) _STC_MACRO_VAR(                                         \
            _s_) = { malloc(sizeof(*(v).__stc_vec_data) * (v).len), (v).len }; \
        if ((v).len)                                                           \
            memcpy(_STC_MACRO_VAR(_s_).__stc_slice_data, (v).__stc_vec_data,   \
                   sizeof(*(v).__stc_vec_data) * (v).len);                     \
        _STC_FATP_AUTO_CAST_VAL_TO_VAL(StcVec, StcSlice, v,                    \
                                       _STC_MACRO_VAR(_s_));                   \
    })

/**
 * Initialise a vector with a given capacity and allocator.
 *
 * @param[in,out] vec  the vector to initialise
 * @param[in]     size the size of a single element in the vector
 * @param[in]     cap  the capacity to the initialise the vector with
 */
void _stc_vec_init(StcVec(void) *vec, size_t size, size_t cap);

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
