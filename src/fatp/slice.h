#ifndef STC_SLICE_H
#define STC_SLICE_H

#include <stddef.h>

#ifdef STC_SLICE_ENABLE_SHORT_NAMES
#    define slice_header     stc_slice_header
#    define slice_new        stc_slice_new
#    define slice_from_parts stc_slice_from_parts
#    define slice_len        stc_slice_len
#    define slice_len_unsafe stc_slice_len_unsafe
#endif /* STC_SLICE_ENABLE_SHORT_NAMES */

typedef struct {
    size_t len;
} StcSliceHeader;

#define stc_slice_header(s) (((StcSliceHeader *) s) - 1)

#define stc_slice_new(size, len) _stc_slice_from_parts(NULL, (size), (len))
#define stc_slice_from_parts(p, len) \
    _stc_slice_from_parts((p), sizeof(*(p)), (len))

#define stc_slice_len(s)        ((s) ? stc_slice_len_unsafe(s) : 0)
#define stc_slice_len_unsafe(s) (stc_slice_header(s)->len)

/**
 * Creates a new slice by copying the data the pointer p points to with element
 * size of size and length of len.
 *
 * @param[in] p the pointer to the data
 * @param[in] size the size of each element
 * @param[in] len the length of (number of elements in) the data
 *
 * @return pointer to the slice data
 */
void *_stc_slice_from_parts(void *p, size_t size, size_t len);

#endif /* STC_SLICE_H */
