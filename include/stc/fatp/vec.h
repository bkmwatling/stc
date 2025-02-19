#ifndef STC_VEC_H
#define STC_VEC_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stc/fatp/slice.h>
#include <stc/util/macro.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_VEC_ENABLE_SHORT_NAMES)
#    define Vec             StcVec
#    define VEC_DEFAULT_CAP STC_VEC_DEFAULT_CAP

#    define vec_new          stc_vec_new
#    define vec_init         stc_vec_init
#    define vec_default      stc_vec_default
#    define vec_default_init stc_vec_default_init
#    define vec_clone        stc_vec_clone
#    define vec_free         stc_vec_free

#    define vec_len      stc_vec_len
#    define vec_cap      stc_vec_cap
#    define vec_is_empty stc_vec_is_empty
#    define vec_clear    stc_vec_clear

#    define vec_push_back  stc_vec_push_back
#    define vec_push_front stc_vec_push_front
#    define vec_pop_back   stc_vec_pop_back
#    define vec_pop_front  stc_vec_pop_front
// #    define vec_ptr_at     stc_vec_ptr_at
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

// TODO: add allocator field once allocator type is defined
struct stc_vec_header {
    size_t cap;
    size_t len;
};

#define _stc_vec_header(v) ((struct stc_vec_header *) (v) - 1)

/**
 * Macro to use a vector with specified element type.
 *
 * NOTE: StcVec(void) can be used to indicate a vector of unknown/any type.
 *
 * @param[in] T the type of the vector elements
 *
 * @return the vector type specifier for the element type
 */
#define StcVec(T) T *

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
#define stc_vec_new(T, cap)                                          \
    __extension__({                                                  \
        __auto_type _STC_MACRO_VAR(_stc_vec_new_cap_) = (cap);       \
        StcVec(T)   _STC_MACRO_VAR(_stc_vec_new_v_) =                \
            (void *) ((struct stc_vec_header *) malloc(              \
                          sizeof(struct stc_vec_header) +            \
                          sizeof(*_STC_MACRO_VAR(_stc_vec_new_v_)) * \
                              _STC_MACRO_VAR(_stc_vec_new_cap_)) +   \
                      1);                                            \
        stc_vec_cap(_STC_MACRO_VAR(_stc_vec_new_v_)) =               \
            _STC_MACRO_VAR(_stc_vec_new_cap_);                       \
        stc_vec_len(_STC_MACRO_VAR(_stc_vec_new_v_)) = 0;            \
        _STC_MACRO_VAR(_stc_vec_new_v_);                             \
    })

/**
 * Initialise a vector's memory to a given capacity, setting the length to 0.
 *
 * @param[in,out] vp  a pointer to the vector to initialise
 * @param[in]     cap the capacity to the initialise the vector with
 */
#define stc_vec_init(vp, cap)                                           \
    __extension__({                                                     \
        __auto_type _STC_MACRO_VAR(_stc_vec_init_vp_)  = (vp);          \
        __auto_type _STC_MACRO_VAR(_stc_vec_init_cap_) = (cap);         \
        *_STC_MACRO_VAR(_stc_vec_init_vp_) =                            \
            (void *) ((struct stc_vec_header *) malloc(                 \
                          sizeof(struct stc_vec_header) +               \
                          sizeof(**_STC_MACRO_VAR(_stc_vec_init_vp_)) * \
                              _STC_MACRO_VAR(_stc_vec_init_cap_)) +     \
                      1);                                               \
        stc_vec_cap(*_STC_MACRO_VAR(_stc_vec_init_vp_)) =               \
            _STC_MACRO_VAR(_stc_vec_init_cap_);                         \
        stc_vec_len(*_STC_MACRO_VAR(_stc_vec_init_vp_)) = 0;            \
    })

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
 * @param[in,out] vp a pointer to the vector to initialise with default capacity
 */
#define stc_vec_default_init(vp) stc_vec_init(vp, STC_VEC_DEFAULT_CAP)

/**
 * Create a (shallow) clone of a vector by copying the underlying data.
 *
 * @param[in] v the vector to clone
 *
 * @return the clone of the vector
 */
#define stc_vec_clone(v)                                                \
    __extension__({                                                     \
        __auto_type _STC_MACRO_VAR(_stc_vec_clone_v_) = (v);            \
        __auto_type _STC_MACRO_VAR(_stc_vec_clone_w_) =                 \
            _STC_MACRO_VAR(_stc_vec_clone_v_);                          \
        stc_vec_init(&_STC_MACRO_VAR(_stc_vec_clone_w_),                \
                     stc_vec_len(_STC_MACRO_VAR(_stc_vec_clone_v_)));   \
        stc_vec_len(_STC_MACRO_VAR(_stc_vec_clone_w_)) =                \
            stc_vec_cap(_STC_MACRO_VAR(_stc_vec_clone_w_));             \
        if (stc_vec_len(_STC_MACRO_VAR(_stc_vec_clone_w_)))             \
            memcpy(_STC_MACRO_VAR(_stc_vec_clone_w_),                   \
                   _STC_MACRO_VAR(_stc_vec_clone_v_),                   \
                   sizeof(*_STC_MACRO_VAR(_stc_vec_clone_w_)) *         \
                       stc_vec_len(_STC_MACRO_VAR(_stc_vec_clone_w_))); \
        _STC_MACRO_VAR(_stc_vec_clone_w_);                              \
    })

/**
 * Free the memory of a vector.
 *
 * NOTE: The underlying elements are not freed by this macro, only the array of
 *       elements. Cleaning up memory of each element is left to the caller.
 *
 * @param[in] v the vector to free the memory of
 */
#define stc_vec_free(v) free(_stc_vec_header(v));

/**
 * Get the length of a vector.
 *
 * @param[in] v the vector to get the length of
 *
 * @return the length of the vector
 */
#define stc_vec_len(v) _stc_vec_header(v)->len

/**
 * Get the capacity of a vector.
 *
 * @param[in] v the vector to get the capacity of
 *
 * @return the capacity of the vector
 */
#define stc_vec_cap(v) _stc_vec_header(v)->cap

/**
 * Check whether a vector is empty (has a length of 0).
 *
 * @param[in] v the vector to check if empty
 *
 * @return a non-zero value if the vector is empty; else 0
 */
#define stc_vec_is_empty(v) (stc_vec_len(v) == 0)

/**
 * Clear a vector to have a length of 0.
 *
 * NOTE: The underlying elements are not freed by this macro. Cleaning up memory
 *       is left to the caller.
 *
 * @param[in,out] v a pointer to the vector to clear
 */
#define stc_vec_clear(v) (stc_vec_len(*v) = 0)

/**
 * Add an element to the tail of a vector.
 *
 * @param[in,out] vp a pointer to the vector to add the element to
 * @param[in]     x  the element to add to the vector
 */
#define stc_vec_push_back(vp, x)                                    \
    __extension__({                                                 \
        __auto_type _STC_MACRO_VAR(_stc_vec_push_back_vp_) = (vp);  \
        stc_vec_reserve(_STC_MACRO_VAR(_stc_vec_push_back_vp_), 1); \
        (*_STC_MACRO_VAR(_stc_vec_push_back_vp_))[stc_vec_len(      \
            *_STC_MACRO_VAR(_stc_vec_push_back_vp_))++] = (x);      \
    })

/**
 * Add an element to the head of a vector.
 *
 * @param[in,out] vp a pointer to the vector to add the element to
 * @param[in]     x  the element to add to the vector
 */
#define stc_vec_push_front(vp, x) stc_vec_insert(vp, 0, x)

/**
 * Remove and return the element at the tail of a vector.
 *
 * @param[in,out] vp a pointer to the vector to pop the tail element from
 *
 * @return the tail element of the vector
 */
#define stc_vec_pop_back(vp)                                      \
    __extension__({                                               \
        __auto_type _STC_MACRO_VAR(_stc_vec_pop_back_vp_) = (vp); \
        (*_STC_MACRO_VAR(_stc_vec_pop_back_vp_))[--stc_vec_len(   \
            *_STC_MACRO_VAR(_stc_vec_pop_back_vp_))];             \
    })

/**
 * Remove and return the element at the head of a vector.
 *
 * @param[in,out] vp a pointer to the vector to pop the head element from
 *
 * @return the head element of the vector
 */
#define stc_vec_pop_front(vp)                                              \
    __extension__({                                                        \
        __auto_type _STC_MACRO_VAR(_stc_vec_pop_front_vp_) = (vp);         \
        __auto_type _STC_MACRO_VAR(_stc_vec_pop_front_x_) =                \
            (*_STC_MACRO_VAR(_stc_vec_pop_front_vp_))[0];                  \
        _stc_vec_shift(                                                    \
            (StcVec(void) *) _STC_MACRO_VAR(_stc_vec_pop_front_vp_), 1, 0, \
            sizeof(**_STC_MACRO_VAR(_stc_vec_pop_front_vp_)));             \
        stc_vec_len(*_STC_MACRO_VAR(_stc_vec_pop_front_vp_))--;            \
        _STC_MACRO_VAR(_stc_vec_pop_front_x_);                             \
    })

/**
 * Get the element of a vector by pointer at a specified index.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] vp a pointer to the vector to get the indexed element from
 * @param[in] i  the index of the element to retrieve
 *
 * @return the indexed element from the vector
 */
// #define stc_vec_ptr_at(vp, i) (*(vp))[i]

/**
 * Get the first element from a vector.
 *
 * @param[in] v the vector to retrieve the first element of
 *
 * @return the first element of the vector
 */
#define stc_vec_first(v) (v)[0]

/**
 * Get the last element from a vector.
 *
 * @param[in] v the vector to retrieve the last element of
 *
 * @return the last element of the vector
 */
#define stc_vec_last(v)                                                       \
    __extension__({                                                           \
        __auto_type _STC_MACRO_VAR(_stc_vec_last_v_) = (v);                   \
        (_STC_MACRO_VAR(                                                      \
            _stc_vec_last_v_)[stc_vec_len(_STC_MACRO_VAR(_stc_vec_last_v_)) - \
                              1]);                                            \
    })

/**
 * Insert an element into a vector at the specified index.
 *
 * @param[in,out] vp a pointer to the vector to insert the element into
 * @param[in]     i  the index where to insert the element
 * @param[in]     x  the element to insert into the vector
 */
#define stc_vec_insert(vp, i, x)                                             \
    __extension__({                                                          \
        __auto_type _STC_MACRO_VAR(_stc_vec_insert_vp_) = (vp);              \
        __auto_type _STC_MACRO_VAR(_stc_vec_insert_i_)  = (i);               \
        stc_vec_reserve_index(_STC_MACRO_VAR(_stc_vec_insert_vp_),           \
                              _STC_MACRO_VAR(_stc_vec_insert_i_), 1);        \
        stc_vec_len(_STC_MACRO_VAR(_stc_vec_insert_vp_))++;                  \
        (*_STC_MACRO_VAR(                                                    \
            _stc_vec_insert_vp_))[_STC_MACRO_VAR(_stc_vec_insert_i_)] = (x); \
    })

/**
 * Remove and return the element at the specified index from a vector.
 *
 * @param[in,out] vp a pointer to the vector to remove the element from
 * @param[in]     i  the index of the element to remove
 *
 * @return the element removed from the vector
 */
#define stc_vec_remove(vp, i)                                               \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_vec_remove_vp_) = (vp);             \
        __auto_type _STC_MACRO_VAR(_stc_vec_remove_i_)  = (i);              \
        __auto_type _STC_MACRO_VAR(_stc_vec_remove_x_)  = (*_STC_MACRO_VAR( \
            _stc_vec_remove_vp_))[_STC_MACRO_VAR(_stc_vec_remove_i_)];     \
        stc_vec_drain(_STC_MACRO_VAR(_stc_vec_remove_vp_),                  \
                      _STC_MACRO_VAR(_stc_vec_remove_i_), 1);               \
        _STC_MACRO_VAR(_stc_vec_remove_x_);                                 \
    })

/**
 * Remove and return the element at the specified index from a vector, by
 * swapping it with the last element of the vector for efficiency.
 *
 * This macro is useful when the order of the elements in the vector don't
 * matter, as it is far more efficient to swap a single element than it is to
 * shift the entire underlying memory.
 *
 * @param[in,out] vp a pointer to the vector to remove the element from
 * @param[in]     i  the index of the element to remove
 *
 * @return the element removed from the vector
 */
#define stc_vec_swap_remove(vp, i)                                      \
    __extension__({                                                     \
        __auto_type _STC_MACRO_VAR(_stc_vec_swap_remove_vp_) = (vp);    \
        __auto_type _STC_MACRO_VAR(_stc_vec_swap_remove_i_)  = (i);     \
        __auto_type _STC_MACRO_VAR(_stc_vec_swap_remove_x_) =           \
            (*_STC_MACRO_VAR(_stc_vec_swap_remove_vp_))[_STC_MACRO_VAR( \
                _stc_vec_swap_remove_i_)];                              \
        (*_STC_MACRO_VAR(_stc_vec_swap_remove_vp_))[_STC_MACRO_VAR(     \
            _stc_vec_swap_remove_i_)] =                                 \
            stc_vec_pop_back(_STC_MACRO_VAR(_stc_vec_swap_remove_vp_)); \
        _STC_MACRO_VAR(_stc_vec_swap_remove_x_);                        \
    })

/**
 * Drain (remove) the specified number of elements starting from a specified
 * index from a vector.
 *
 * NOTE: The underlying drained elements are not freed by this macro. Cleaning
 *       up memory is left to the caller.
 *
 * @param[in,out] vp a pointer to the vector to drain the elements from
 * @param[in]     i  the index to drain the elements from
 * @param[in]     n  the number of elements to drain
 */
#define stc_vec_drain(vp, i, n)                                             \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_vec_drain_vp_) = (vp);              \
        __auto_type _STC_MACRO_VAR(_stc_vec_drain_i_)  = (i);               \
        __auto_type _STC_MACRO_VAR(_stc_vec_drain_n_)  = (n);               \
        _stc_vec_shift((StcVec(void) *) _STC_MACRO_VAR(_stc_vec_drain_vp_), \
                       _STC_MACRO_VAR(_stc_vec_drain_i_) +                  \
                           _STC_MACRO_VAR(_stc_vec_drain_n_),               \
                       _STC_MACRO_VAR(_stc_vec_drain_i_),                   \
                       sizeof(**_STC_MACRO_VAR(_stc_vec_drain_vp_)));       \
        stc_vec_len(_STC_MACRO_VAR(_stc_vec_drain_vp_)) -=                  \
            _STC_MACRO_VAR(_stc_vec_drain_n_);                              \
    })

/**
 * Truncate the vector to the specified length, removing the elements between
 * the specified length and the length of the vector.
 *
 * NOTE: The vector is only truncated if the specified length is between 0 and
 *       the length of the vector (non-inclusive). The only case where 0 is
 *       returned by this macro is when this condition is not met.
 * NOTE: The underlying removed elements are not freed by this macro. Cleaning
 *       up memory is left to the caller.
 *
 * @param[in,out] vp  a pointer to the vector to truncate
 * @param[in]     len the length to truncate the vector to
 *
 * @return a non-zero value if the vector was truncated; else 0
 */
#define stc_vec_truncate(vp, len)                                      \
    __extension__({                                                    \
        __auto_type _STC_MACRO_VAR(_stc_vec_truncate_vp_)  = (vp);     \
        __auto_type _STC_MACRO_VAR(_stc_vec_truncate_len_) = (len);    \
        0 <= _STC_MACRO_VAR(_stc_vec_truncate_len_) &&                 \
                _STC_MACRO_VAR(_stc_vec_truncate_len_) <               \
                    stc_vec_len(_STC_MACRO_VAR(_stc_vec_truncate_vp_)) \
            ? (stc_vec_len(_STC_MACRO_VAR(_stc_vec_truncate_vp_)) =    \
                   _STC_MACRO_VAR(_stc_vec_truncate_len_),             \
               1)                                                      \
            : 0;                                                       \
    })

/**
 * Move all the elements from a vector to the end of another vector.
 *
 * NOTE: After the append, the second vector will be empty.
 *
 * @param[in,out] vp a pointer to the vector to move elements to the end of
 * @param[in,out] wp a pointer to the vector to move the elements from
 */
#define stc_vec_append(vp, wp)                                             \
    __extension__({                                                        \
        __auto_type _STC_MACRO_VAR(_stc_vec_append_wp_) = (wp);            \
        stc_vec_extend(vp, *_STC_MACRO_VAR(_stc_vec_append_wp_),           \
                       stc_vec_len(*_STC_MACRO_VAR(_stc_vec_append_wp_))); \
        stc_vec_clear(_STC_MACRO_VAR(_stc_vec_append_wp_));                \
    })

/**
 * Extend a vector (append to its tail) with the elements from an array with
 * specified length.
 *
 * @param[in,out] vp  a pointer to the vector to extend
 * @param[in]     p   the array of elements to add to the vector
 * @param[in]     len the length of the array being extended from
 */
#define stc_vec_extend(vp, p, len)                                            \
    __extension__({                                                           \
        __auto_type _STC_MACRO_VAR(_stc_vec_extend_vp_) = (vp);               \
        __auto_type _STC_MACRO_VAR(_stc_vec_extend_p_)  = (p);                \
        STC_TYPECHECK(__typeof__(*_STC_MACRO_VAR(_stc_vec_extend_vp_)),       \
                      _STC_MACRO_VAR(_stc_vec_extend_p_));                    \
        _stc_vec_extend((StcVec(void) *) _STC_MACRO_VAR(_stc_vec_extend_vp_), \
                        _STC_MACRO_VAR(_stc_vec_extend_p_),                   \
                        sizeof(*_STC_MACRO_VAR(_stc_vec_extend_p_)), (len));  \
    })

/**
 * Extend a vector (append to its tail) with the elements from a slice.
 *
 * @param[in,out] vp a pointer to the vector to extend
 * @param[in]     s  the slice of elements to add to the vector
 */
#define stc_vec_extend_from_slice(vp, s)                                   \
    __extension__({                                                        \
        __auto_type _STC_MACRO_VAR(_stc_vec_extend_from_slice_s_) = (s);   \
        stc_vec_extend(                                                    \
            vp, _STC_MACRO_VAR(_stc_vec_extend_from_slice_s_),             \
            stc_slice_len(_STC_MACRO_VAR(_stc_vec_extend_from_slice_s_))); \
    })

/**
 * Reserve enough space in a vector for the specified number of elements.
 *
 * NOTE: More space than may be allocated than may be necessary.
 *
 * @param[in,out] vp a pointer to the vector to reserve space for
 * @param[in]     n  the number of elements to reserve space for in the vector
 */
#define stc_vec_reserve(vp, n)                                     \
    __extension__({                                                \
        __auto_type _STC_MACRO_VAR(_stc_vec_reserve_vp_) = (vp);   \
        _stc_vec_reserve(                                          \
            (StcVec(void) *) _STC_MACRO_VAR(_stc_vec_reserve_vp_), \
            sizeof(**_STC_MACRO_VAR(_stc_vec_reserve_vp_)), (n));  \
    })

/**
 * Reserve exactly enough space in a vector for the specified number of
 * elements.
 *
 * NOTE: The exact amount of space necessary is allocated.
 *
 * @param[in,out] vp the vector to reserve exact space for
 * @param[in]     n  the number of elements to reserve exact space for in the
 *                   vector
 */
#define stc_vec_reserve_exact(vp, n)                                     \
    __extension__({                                                      \
        __auto_type _STC_MACRO_VAR(_stc_vec_reserve_exact_vp_) = (vp);   \
        _stc_vec_reserve_exact(                                          \
            (StcVec(void) *) _STC_MACRO_VAR(_stc_vec_reserve_exact_vp_), \
            sizeof(**_STC_MACRO_VAR(_stc_vec_reserve_exact_vp_)), (n));  \
    })

/**
 * Reserve enough space in a vector for the specified number of elements at the
 * specified index.
 *
 * The space is allocated at the specified index, essentially leaving a gap in
 * the vector at that index.
 * NOTE: More space than may be allocated than may be necessary.
 *
 * @param[in,out] vp the vector to reserve space for
 * @param[in]     i  the index to reserve space at
 * @param[in]     n  the number of elements to reserve space for in the vector
 */
#define stc_vec_reserve_index(vp, i, n)                                  \
    __extension__({                                                      \
        __auto_type _STC_MACRO_VAR(_stc_vec_reserve_index_vp_) = (vp);   \
        __auto_type _STC_MACRO_VAR(_stc_vec_reserve_index_i_)  = (i);    \
        __auto_type _STC_MACRO_VAR(_stc_vec_reserve_index_n_)  = (n);    \
        stc_vec_reserve(_STC_MACRO_VAR(_stc_vec_reserve_index_vp_),      \
                        _STC_MACRO_VAR(_stc_vec_reserve_index_n_));      \
        _stc_vec_shift(                                                  \
            (StcVec(void) *) _STC_MACRO_VAR(_stc_vec_reserve_index_vp_), \
            _STC_MACRO_VAR(_stc_vec_reserve_index_i_),                   \
            _STC_MACRO_VAR(_stc_vec_reserve_index_i_) +                  \
                _STC_MACRO_VAR(_stc_vec_reserve_index_n_),               \
            sizeof(**_STC_MACRO_VAR(_stc_vec_reserve_index_vp_)));       \
    })

/**
 * Shrink a vector to the specified capacity.
 *
 * NOTE: The vector will only shrink to its length at minimum, and will only
 *       shrink if the specified capacity is less than the vector's capacity.
 *
 * @param[in,out] vp  a pointer to the vector to shrink
 * @param[in]     cap the capacity to shrink the vector to
 */
#define stc_vec_shrink(vp, cap)                                                \
    __extension__({                                                            \
        __auto_type _STC_MACRO_VAR(_stc_vec_shrink_vp_) = (vp);                \
        _stc_vec_shrink((StcVec(void) *) _STC_MACRO_VAR(_stc_vec_shrink_vp_),  \
                        sizeof(**_STC_MACRO_VAR(_stc_vec_shrink_vp_)), (cap)); \
    })

/**
 * Shrink a vector so that it's capacity and length are the same.
 *
 * @param[in,out] vp a pointer to the vector to shrink to its length
 */
#define stc_vec_shrink_to_fit(vp)                                      \
    __extension__({                                                    \
        __auto_type _STC_MACRO_VAR(_stc_vec_shrink_to_fit_vp_) = (vp); \
        stc_vec_shrink(                                                \
            _STC_MACRO_VAR(_stc_vec_shrink_to_fit_vp_),                \
            stc_vec_len(*_STC_MACRO_VAR(_stc_vec_shrink_to_fit_vp_))); \
    })

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
#define stc_vec_as_slice(v) (v)

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
#define stc_vec_to_slice(v)                                     \
    __extension__({                                             \
        __auto_type _STC_MACRO_VAR(_stc_vec_to_slice_v_) = (v); \
        stc_slice_from_parts(                                   \
            _STC_MACRO_VAR(_stc_vec_to_slice_v_),               \
            stc_vec_len(_STC_MACRO_VAR(_stc_vec_to_slice_v_))); \
    })

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

#endif /* STC_VEC_H */
