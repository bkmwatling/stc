#ifndef STC_SLICE_H
#define STC_SLICE_H

#include <stddef.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SLICE_ENABLE_SHORT_NAMES)
#    define slice_header     stc_slice_header
#    define slice_new        stc_slice_new
#    define slice_init       stc_slice_init
#    define slice_from_parts stc_slice_from_parts
#    define slice_clone      stc_slice_clone
#    define slice_free       stc_slice_free

#    define slice_len        stc_slice_len
#    define slice_len_unsafe stc_slice_len_unsafe
#    define slice_first      stc_slice_first
#    define slice_last       stc_slice_last
#endif /* STC_SLICE_ENABLE_SHORT_NAMES */

typedef struct {
    size_t len;
} StcSliceHeader;

#define stc_slice_header(s) (((StcSliceHeader *) s) - 1)

#define stc_slice_new(size, len) _stc_slice_from_parts(NULL, (size), (len))
#define stc_slice_init(s, len)   ((s) = stc_slice_new(sizeof(*(s)), (len)))
#define stc_slice_from_parts(p, len) \
    _stc_slice_from_parts((p), sizeof(*(p)), (len))
#define stc_slice_clone(s) stc_slice_from_parts(s, stc_slice_len(s))

#define stc_slice_len(s)        ((s) ? stc_slice_len_unsafe(s) : 0)
#define stc_slice_len_unsafe(s) (stc_slice_header(s)->len)
#define stc_slice_first(s)      ((s)[0])
#define stc_slice_last(s)       ((s)[stc_slice_len_unsafe(v) - 1])

/**
 * Frees the memory allocated for a slice.
 *
 * @param[in] slice the pointer to the slice to free
 */
void stc_slice_free(void *slice);

/**
 * Creates a new slice by copying the data that pointer p points to with element
 * size of size and length of len. If p is NULL, then the contents of the
 * returned slice is left uninitialised.
 *
 * @param[in] p    the pointer to the data to copy
 * @param[in] size the size of each element
 * @param[in] len  the length of (number of elements in) the data
 *
 * @return a pointer to the slice data
 */
void *_stc_slice_from_parts(const void *p, size_t size, size_t len);

#endif /* STC_SLICE_H */
