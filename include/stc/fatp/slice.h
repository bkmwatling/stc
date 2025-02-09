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
#    define slice_clone      stc_slice_clone
#    define slice_free       stc_slice_free

#    define slice_at    stc_slice_at
#    define slice_first stc_slice_first
#    define slice_last  stc_slice_last
#endif /* STC_SLICE_ENABLE_SHORT_NAMES */

/**
 * Simple macro to show intention of using slice type.
 *
 * NOTE: StcSlice(void) can be used to indicate a slice of unknown/any type.
 */
#define StcSlice(T) STC_CONCAT(__stc_slice_, _STC_FATP_MAGIC_WORD_MACRO(T))

// TODO: add allocator field once allocator type is defined
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
#define stc_slice_new(T, len)                                        \
    (StcSlice(T))                                                    \
    {                                                                \
        malloc(sizeof(_STC_FATP_MAGIC_TYPE_MACRO(T)) * (len)), (len) \
    }
#define stc_slice_init(s, len)                                               \
    ((s)->__stc_slice_data = malloc(sizeof(*(s)->__stc_slice_data) * (len)), \
     (s)->len              = (len))

/**
 * Create a new slice by copying the data that pointer p points to with type T
 * and length of len.
 *
 * NOTE: p is expected to be not be NULL, and the type T must match the type of
 * the data pointed to by p.
 *
 * @param[in] T   the type of the slice to construct
 * @param[in] p   the pointer to the data to copy
 * @param[in] len the length of (number of elements in) the data
 *
 * @return a slice initialised with a copy of the data pointed to by p
 */
#define stc_slice_from_parts(T, p, len)                                       \
    ({                                                                        \
        STC_TYPECHECK(T, *(p));                                               \
        StcSlice(T) _STC_MACRO_VAR(_s_) = stc_slice_new(T, len);              \
        memcpy(_STC_MACRO_VAR(_s_).__stc_slice_data, (p), sizeof(T) * (len)); \
        _STC_MACRO_VAR(_s_);                                                  \
    })
#define stc_slice_clone(s)                                                     \
    ({                                                                         \
        __typeof__(s) _STC_MACRO_VAR(_s_) = {                                  \
            malloc(sizeof(*(s).__stc_slice_data) * (s).len), (s).len           \
        };                                                                     \
        if ((s).len)                                                           \
            memcpy(_STC_MACRO_VAR(_s_).__stc_slice_data, (s).__stc_slice_data, \
                   sizeof(*(s).__stc_slice_data) * (s).len);                   \
        _STC_MACRO_VAR(_s_);                                                   \
    })
#define stc_slice_free(s) free((s).__stc_slice_data)

#define stc_slice_at(s, i) ((s).__stc_slice_data[i])
#define stc_slice_first(s) stc_slice_at(s, 0)
#define stc_slice_last(s)  stc_slice_at(s, (s).len - 1)

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
