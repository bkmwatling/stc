#ifndef STC_SLICE_H
#define STC_SLICE_H

#include <stdlib.h>
#include <string.h>

#include <stc/util/macro.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SLICE_ENABLE_SHORT_NAMES)
#    define Slice StcSlice

#    define slice_new        stc_slice_new
#    define slice_init       stc_slice_init
#    define slice_from_parts stc_slice_from_parts
#    define slice_from_range stc_slice_from_range
#    define slice_clone      stc_slice_clone
#    define slice_free       stc_slice_free

#    define slice_len   stc_slice_len
// #    define slice_ptr_at stc_slice_ptr_at
#    define slice_first stc_slice_first
#    define slice_last  stc_slice_last

#    define slice_subslice       stc_slice_subslice
#    define slice_subslice_from  stc_slice_subslice_from
#    define slice_subslice_until stc_slice_subslice_until
#endif /* STC_SLICE_ENABLE_SHORT_NAMES */

// TODO: add allocator field once allocator type is defined
struct stc_slice_header {
    size_t len;
};

#define _stc_slice_header(s) ((struct stc_slice_header *) (s) - 1)

/**
 * Macro to use a slice with specified element type.
 *
 * NOTE: StcSlice(void) can be used to indicate a slice of unknown/any type.
 *
 * @param[in] T the type of the slice elements
 *
 * @return the slice type specifier for the element type
 */
#define StcSlice(T) T *

// TODO: use allocator instead of malloc and free
/**
 * Create a new slice with given type and length, allocating the necessary
 * underlying memory.
 *
 * @param[in] T   the type of the slice
 * @param[in] len the length of the slice to create
 *
 * @return the created slice
 */
#define stc_slice_new(T, len)                                          \
    __extension__({                                                    \
        __auto_type _STC_MACRO_VAR(_stc_slice_new_len_) = (len);       \
        StcSlice(T) _STC_MACRO_VAR(_stc_slice_new_s_) =                \
            (void *) ((struct stc_slice_header *) malloc(              \
                          sizeof(struct stc_slice_header) +            \
                          sizeof(*_STC_MACRO_VAR(_stc_slice_new_s_)) * \
                              _STC_MACRO_VAR(_stc_slice_new_len_)) +   \
                      1);                                              \
        stc_slice_len(_STC_MACRO_VAR(_stc_slice_new_s_)) =             \
            _STC_MACRO_VAR(_stc_slice_new_len_);                       \
        _STC_MACRO_VAR(_stc_slice_new_s_);                             \
    })

/**
 * Initialise a given slice's memory to a given length.
 *
 * @param[in,out] sp  a pointer to the slice to initialise
 * @param[in]     len the length of the slice to initialise
 */
#define stc_slice_init(sp, len)                                           \
    __extension__({                                                       \
        __auto_type _STC_MACRO_VAR(_stc_slice_init_sp_)  = (sp);          \
        __auto_type _STC_MACRO_VAR(_stc_slice_init_len_) = (len);         \
        *_STC_MACRO_VAR(_stc_slice_init_sp_) =                            \
            (void *) ((struct stc_slice_header *) malloc(                 \
                          sizeof(struct stc_slice_header) +               \
                          sizeof(**_STC_MACRO_VAR(_stc_slice_init_sp_)) * \
                              _STC_MACRO_VAR(_stc_slice_init_len_)) +     \
                      1);                                                 \
        stc_slice_len(*_STC_MACRO_VAR(_stc_slice_init_sp_)) =             \
            _STC_MACRO_VAR(_stc_slice_init_len_);                         \
    })

/**
 * Create a new slice by copying the data from a pointer with specified length.
 *
 * NOTE: p is expected to be not be NULL.
 *
 * @param[in] p   the pointer to the data to copy
 * @param[in] len the length of (number of elements in) the data
 *
 * @return a slice initialised with a copy of the data pointed to by p
 */
#define stc_slice_from_parts(p, len)                                          \
    __extension__({                                                           \
        __auto_type    _STC_MACRO_VAR(_stc_slice_from_parts_p_)   = (p);      \
        __auto_type    _STC_MACRO_VAR(_stc_slice_from_parts_len_) = (len);    \
        StcSlice(void) _STC_MACRO_VAR(_stc_slice_from_parts_s_) =             \
            (void *) ((struct stc_slice_header *) malloc(                     \
                          sizeof(struct stc_slice_header) +                   \
                          sizeof(*_STC_MACRO_VAR(_stc_slice_from_parts_p_)) * \
                              _STC_MACRO_VAR(_stc_slice_from_parts_len_)) +   \
                      1);                                                     \
        stc_slice_len(_STC_MACRO_VAR(_stc_slice_from_parts_s_)) =             \
            _STC_MACRO_VAR(_stc_slice_from_parts_len_);                       \
        if (_STC_MACRO_VAR(_stc_slice_from_parts_len_))                       \
            memcpy(_STC_MACRO_VAR(_stc_slice_from_parts_s_),                  \
                   _STC_MACRO_VAR(_stc_slice_from_parts_p_),                  \
                   sizeof(*_STC_MACRO_VAR(_stc_slice_from_parts_p_)) *        \
                       _STC_MACRO_VAR(_stc_slice_from_parts_len_));           \
        _STC_MACRO_VAR(_stc_slice_from_parts_s_);                             \
    })

/**
 * Create a slice of the range between two pointers by copying that range of
 * memory.
 *
 * NOTE: start and end are expected to be not be NULL.
 *
 * @param[in] start the start (pointer) of the range
 * @param[in] end   the end (pointer) of the range
 *
 * @return a slice of the copied array defined by the range
 */
#define stc_slice_from_range(start, end)                                    \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_slice_from_range_start_) = (start); \
        stc_slice_from_parts(                                               \
            _STC_MACRO_VAR(_stc_slice_from_range_start_),                   \
            (end) - _STC_MACRO_VAR(_stc_slice_from_range_start_));          \
    })

/**
 * Create a (shallow) clone of a slice by copying the underlying data.
 *
 * @param[in] s the slice to clone
 *
 * @return the clone of the slice
 */
#define stc_slice_clone(s)                                                  \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_slice_clone_s_) = (s);              \
        __auto_type _STC_MACRO_VAR(_stc_slice_clone_t_) =                   \
            _STC_MACRO_VAR(_stc_slice_clone_s_);                            \
        stc_slice_init(&_STC_MACRO_VAR(_stc_slice_clone_t_),                \
                       stc_slice_len(_STC_MACRO_VAR(_stc_slice_clone_s_))); \
        if (stc_slice_len(_STC_MACRO_VAR(_stc_slice_clone_t_)))             \
            memcpy(_STC_MACRO_VAR(_stc_slice_clone_t_),                     \
                   _STC_MACRO_VAR(_stc_slice_clone_s_),                     \
                   sizeof(*_STC_MACRO_VAR(_stc_slice_clone_t_)) *           \
                       stc_slice_len(_STC_MACRO_VAR(_stc_slice_clone_t_))); \
        _STC_MACRO_VAR(_stc_slice_clone_t_);                                \
    })

/**
 * Free the memory of a slice.
 *
 * @param[in] s the slice to free the memory of
 */
#define stc_slice_free(s) free(_stc_slice_header(s))

/**
 * Get the length of a slice.
 *
 * @param[in] s the slice to get the length of
 *
 * @return the length of the slice
 */
#define stc_slice_len(s) _stc_slice_header(s)->len

/**
 * Get the element of a slice by pointer at a specified index.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] sp a pointer to the slice to get the indexed element from
 * @param[in] i  the index of the element to retrieve
 *
 * @return the indexed element from the slice
 */
// #define stc_slice_ptr_at(sp, i) (*(sp))[i]

/**
 * Get the first element from a slice.
 *
 * @param[in] s the slice to retrieve the first element of
 *
 * @return the first element of the slice
 */
#define stc_slice_first(s) (s)[0]

/**
 * Get the last element from a slice.
 *
 * @param[in] s the slice to retrieve the last element of
 *
 * @return the last element of the slice
 */
#define stc_slice_last(s)                                              \
    __extension__({                                                    \
        __auto_type _STC_MACRO_VAR(_stc_slice_last_s_) = (s);          \
        (_STC_MACRO_VAR(_stc_slice_last_s_)                            \
             [stc_slice_len(_STC_MACRO_VAR(_stc_slice_last_s_)) - 1]); \
    })

/**
 * Create a subslice from a slice from the starting index until the ending index
 * (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The subslice is a copy of the slice over the defined range, and thus
 *       needs to be freed.
 *
 * @param[in] s     the slice to create the subslice from
 * @param[in] start the start index of the subslice
 * @param[in] end   the end index of the subslice
 *
 * @return a subslice over the defined range
 */
#define stc_slice_subslice(s, start, end)                                   \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_slice_clone_s_) = (s);              \
        __auto_type _STC_MACRO_VAR(_stc_slice_clone_t_) =                   \
            _STC_MACRO_VAR(_stc_slice_clone_s_);                            \
        __auto_type _STC_MACRO_VAR(_stc_slice_subslice_start_) = (start);   \
        stc_slice_init(&_STC_MACRO_VAR(_stc_slice_clone_t_),                \
                       (end) - _STC_MACRO_VAR(_stc_slice_subslice_start_)); \
        if (stc_slice_len(_STC_MACRO_VAR(_stc_slice_clone_t_)))             \
            memcpy(_STC_MACRO_VAR(_stc_slice_clone_t_),                     \
                   &_STC_MACRO_VAR(_stc_slice_clone_s_)[_STC_MACRO_VAR(     \
                       _stc_slice_subslice_start_)],                        \
                   sizeof(*_STC_MACRO_VAR(_stc_slice_clone_t_)) *           \
                       stc_slice_len(_STC_MACRO_VAR(_stc_slice_clone_t_))); \
        _STC_MACRO_VAR(_stc_slice_clone_t_);                                \
    })

/**
 * Create a subslice from a slice from the starting index until the end of the
 * slice.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The subslice is a copy of the slice over the defined range, and thus
 *       needs to be freed.
 *
 * @param[in] s     the slice to create the subslice from
 * @param[in] start the start index of the subslice
 *
 * @return a subslice from the starting index until the end of the slice
 */
#define stc_slice_subslice_from(s, start)                                \
    __extension__({                                                      \
        __auto_type _STC_MACRO_VAR(_stc_slice_subslice_from_s_) = (s);   \
        stc_slice_subslice(                                              \
            _STC_MACRO_VAR(_stc_slice_subslice_from_s_), start,          \
            stc_slice_len(_STC_MACRO_VAR(_stc_slice_subslice_from_s_))); \
    })

/**
 * Create a subslice from a slice from the start of the slice until the ending
 * index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The subslice is a copy of the slice over the defined range, and thus
 *       needs to be freed.
 *
 * @param[in] s   the slice to create the subslice from
 * @param[in] end the end index of the subslice
 *
 * @return a subslice from the start of the slice until the ending index
 *         (non-inclusive)
 */
#define stc_slice_subslice_until(s, end) stc_slice_subslice(s, 0, end)

#endif /* STC_SLICE_H */
