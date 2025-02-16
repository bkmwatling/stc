#ifndef STC_SLICE_H
#define STC_SLICE_H

#include <stdlib.h>
#include <string.h>

#include <stc/fatp/util.h>
#include <stc/util/macro.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SLICE_ENABLE_SHORT_NAMES)
#    define Slice            StcSlice
#    define slice_define_for stc_slice_define_for

#    define slice_new        stc_slice_new
#    define slice_init       stc_slice_init
#    define slice_from_parts stc_slice_from_parts
#    define slice_from_range stc_slice_from_range
#    define slice_clone      stc_slice_clone
#    define slice_free       stc_slice_free

#    define slice_at    stc_slice_at
#    define slice_first stc_slice_first
#    define slice_last  stc_slice_last

#    define slice_subslice       stc_slice_subslice
#    define slice_subslice_from  stc_slice_subslice_from
#    define slice_subslice_until stc_slice_subslice_until
#endif /* STC_SLICE_ENABLE_SHORT_NAMES */

/**
 * Macro to use a slice with specified element type.
 *
 * NOTE: StcSlice(void) can be used to indicate a slice of unknown/any type.
 * NOTE: Due to C's limited macro system, pointers are specified with the word
 *       `ptr` instead of the * symbol.
 *
 * @param[in] T the type of the slice elements
 *
 * @return the slice type specifier for the element type
 */
#define StcSlice(T) STC_CONCAT(__stc_slice_, _STC_FATP_MAGIC_WORD_MACRO(T))

// TODO: add allocator field once allocator type is defined
/**
 * Macro to define a slice with underlying element type.
 *
 * NOTE: Slices have been defined for all primitive types (including C99
 *       stdint.h), but must be defined for any other types you wish to use.
 * NOTE: Due to C's limited macro system, pointers are specified with the word
 *       `ptr` instead of the * symbol.
 *
 * @param[in] T the type of the slice elements
 */
#define stc_slice_define_for(T)                           \
    typedef struct {                                      \
        _STC_FATP_MAGIC_TYPE_MACRO(T) * __stc_slice_data; \
        size_t len;                                       \
    } StcSlice(T)

/* NOTE: this is purely to indicate **any** type, and is an example of how to
 * define new types for StcSlice. The definition allows the use of
 * StcSlice(void) which means the elements of the slice are of type 'void'
 * (which is obviously useless, but serves well for casting to mean any type of
 * elements). See the bottom of this file for definitions of builtin types for
 * StcSlice. */
stc_slice_define_for(void);

#define _STC_SLICE_ANY(s)     (*(StcSlice(void) *) &(s))
#define _STC_SLICE_PTR_ANY(s) ((StcSlice(void) *) (s))

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
#define stc_slice_new(T, len)                                         \
    ({                                                                \
        __auto_type _STC_MACRO_VAR(_stc_len_) = (len);                \
        (StcSlice(T)){ malloc(sizeof(_STC_FATP_MAGIC_TYPE_MACRO(T)) * \
                              _STC_MACRO_VAR(_stc_len_)),             \
                       _STC_MACRO_VAR(_stc_len_) };                   \
    })

/**
 * Initialise a given slice's memory to a given length.
 *
 * @param[in,out] s   the slice to initialise
 * @param[in]     len the length of the slice to initialise
 */
#define stc_slice_init(s, len)                                          \
    ({                                                                  \
        __auto_type _STC_MACRO_VAR(_stc_s_)   = (s);                    \
        __auto_type _STC_MACRO_VAR(_stc_len_) = (len);                  \
        _STC_MACRO_VAR(_stc_s_)->__stc_slice_data =                     \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_s_)->__stc_slice_data) * \
                   _STC_MACRO_VAR(_stc_len_));                          \
        _STC_MACRO_VAR(_stc_s_)->len = _STC_MACRO_VAR(_stc_len_);       \
    })

/**
 * Create a new slice by copying the data from a pointer with specified type
 * and length.
 *
 * NOTE: p is expected to be not be NULL, and the type T must match the type of
 *       the data pointed to by p.
 *
 * @param[in] T   the type of the slice to construct
 * @param[in] p   the pointer to the data to copy
 * @param[in] len the length of (number of elements in) the data
 *
 * @return a slice initialised with a copy of the data pointed to by p
 */
#define stc_slice_from_parts(T, p, len)                                \
    ({                                                                 \
        __auto_type _STC_MACRO_VAR(_stc_p_)   = (p);                   \
        __auto_type _STC_MACRO_VAR(_stc_len_) = (len);                 \
        STC_TYPECHECK(_STC_FATP_MAGIC_TYPE_MACRO(T),                   \
                      *_STC_MACRO_VAR(_stc_p_));                       \
        StcSlice(T) _STC_MACRO_VAR(_stc_s_) = {                        \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_s_).__stc_slice_data) * \
                   _STC_MACRO_VAR(_stc_len_)),                         \
            _STC_MACRO_VAR(_stc_len_)                                  \
        };                                                             \
        memcpy(_STC_MACRO_VAR(_stc_s_).__stc_slice_data,               \
               _STC_MACRO_VAR(_stc_p_),                                \
               sizeof(*_STC_MACRO_VAR(_stc_s_).__stc_slice_data) *     \
                   _STC_MACRO_VAR(_stc_len_));                         \
        _STC_MACRO_VAR(_stc_s_);                                       \
    })

/**
 * Create a slice of the range between two pointers by copying that range of
 * memory.
 *
 * NOTE: start is expected to be not be NULL, and the type T must match the type
 *       of the data pointed to by start.
 *
 * @param[in] T   the type of the slice to construct
 * @param[in] start the start (pointer) of the range
 * @param[in] end   the end (pointer) of the range
 *
 * @return a slice of the copied array defined by the range
 */
#define stc_slice_from_range(T, start, end)                        \
    ({                                                             \
        __auto_type _STC_MACRO_VAR(_stc_start_) = (start);         \
        stc_slice_from_parts(T, _STC_MACRO_VAR(_stc_start_),       \
                             (end) - _STC_MACRO_VAR(_stc_start_)); \
    })

/**
 * Create a (shallow) clone of a slice by copying the underlying data.
 *
 * @param[in] s the slice to clone
 *
 * @return the clone of the slice
 */
#define stc_slice_clone(s)                                   \
    ({                                                       \
        __auto_type _STC_MACRO_VAR(_stc_s_) = (s);           \
        __auto_type _STC_MACRO_VAR(_stc_data_) =             \
            _STC_MACRO_VAR(_stc_s_).__stc_slice_data;        \
        _STC_MACRO_VAR(_stc_s_).__stc_slice_data =           \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_data_)) *     \
                   _STC_MACRO_VAR(_stc_s_).len);             \
        if (_STC_MACRO_VAR(_stc_s_).len)                     \
            memcpy(_STC_MACRO_VAR(_stc_s_).__stc_slice_data, \
                   _STC_MACRO_VAR(_stc_data_),               \
                   sizeof(*_STC_MACRO_VAR(_stc_data_)) *     \
                       _STC_MACRO_VAR(_stc_s_).len);         \
        _STC_MACRO_VAR(_stc_s_);                             \
    })

/**
 * Free the underlying memory of a slice.
 *
 * @param[in] s the slice to free the underlying memory of
 */
#define stc_slice_free(s) free((s).__stc_slice_data)

/**
 * Get the element of a slice at a specified index.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] s the slice to get the indexed element from
 * @param[in] i the index of the element to retrieve
 *
 * @return the indexed element from the slice
 */
#define stc_slice_at(s, i) ((s).__stc_slice_data[i])

/**
 * Get the first element from a slice.
 *
 * @param[in] s the slice to retrieve the first element of
 *
 * @return the first element of the slice
 */
#define stc_slice_first(s) stc_slice_at(s, 0)

/**
 * Get the last element from a slice.
 *
 * @param[in] s the slice to retrieve the last element of
 *
 * @return the last element of the slice
 */
#define stc_slice_last(s)                              \
    ({                                                 \
        __auto_type _STC_MACRO_VAR(_stc_s_) = (s);     \
        stc_slice_at(_STC_MACRO_VAR(_stc_s_),          \
                     _STC_MACRO_VAR(_stc_s_).len - 1); \
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
#define stc_slice_subslice(s, start, end)                                    \
    ({                                                                       \
        __auto_type _STC_MACRO_VAR(_stc_s_) = (s);                           \
        __auto_type _STC_MACRO_VAR(_stc_data_) =                             \
            _STC_MACRO_VAR(_stc_s_).__stc_slice_data;                        \
        __auto_type _STC_MACRO_VAR(_stc_start_) = (start);                   \
        stc_slice_init(&_STC_MACRO_VAR(_stc_s_),                             \
                       (end) - _STC_MACRO_VAR(_stc_start_));                 \
        if (_STC_MACRO_VAR(_stc_s_).len)                                     \
            memcpy(_STC_MACRO_VAR(_stc_s_).__stc_slice_data,                 \
                   &_STC_MACRO_VAR(_stc_data_)[_STC_MACRO_VAR(_stc_start_)], \
                   sizeof(*_STC_MACRO_VAR(_stc_data_)) *                     \
                       _STC_MACRO_VAR(_stc_s_).len);                         \
        _STC_MACRO_VAR(_stc_s_);                                             \
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
#define stc_slice_subslice_from(s, start)                                    \
    ({                                                                       \
        __auto_type _STC_MACRO_VAR(_stc_s_) = (s);                           \
        __auto_type _STC_MACRO_VAR(_stc_data_) =                             \
            _STC_MACRO_VAR(_stc_s_).__stc_slice_data;                        \
        __auto_type _STC_MACRO_VAR(_stc_start_) = (start);                   \
        stc_slice_init(&_STC_MACRO_VAR(_stc_s_),                             \
                       _STC_MACRO_VAR(_stc_s_).len -                         \
                           _STC_MACRO_VAR(_stc_start_));                     \
        if (_STC_MACRO_VAR(_stc_s_).len)                                     \
            memcpy(_STC_MACRO_VAR(_stc_s_).__stc_slice_data,                 \
                   &_STC_MACRO_VAR(_stc_data_)[_STC_MACRO_VAR(_stc_start_)], \
                   sizeof(*_STC_MACRO_VAR(_stc_data_)) *                     \
                       _STC_MACRO_VAR(_stc_s_).len);                         \
        _STC_MACRO_VAR(_stc_s_);                                             \
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

/* --- Define StcSlice for builtin types ------------------------------------ */

_STC_FATP_DEFINE_FOR_BUILTIN_TYPES(StcSlice, stc_slice_define_for);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    include <stdint.h>
_STC_FATP_DEFINE_FOR_STDC99_TYPES(StcSlice, stc_slice_define_for);
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

#if defined(STC_COMMON_H) && !defined(STC_DISABLE_BASIC_TYPES)
#    if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
_STC_FATP_ALIAS_TYPES_COMMON_FROM_STDC99(StcSlice);
#    else
_STC_FATP_ALIAS_TYPES_COMMON_FROM_BUILTIN(StcSlice);
#    endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */
#endif     /* defined(STC_COMMON_H) && !defined(STC_DISABLE_BASIC_TYPES) */

/* --- End definition of StcSlice for builtin types ------------------------- */

#endif /* STC_SLICE_H */
