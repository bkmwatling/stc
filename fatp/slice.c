#include <stdlib.h>
#include <string.h>

#include "slice.h"

/* TODO: encorporate stc_mem for malloc */
void *_stc_slice_from_parts(void *p, size_t size, size_t len)
{
    StcSliceHeader *slice;

    if (size == 0 || len == 0) return NULL;
    slice      = malloc(size * len + sizeof(StcSliceHeader));
    slice->len = len;
    if (p) memcpy(slice, p, size * len);

    return slice + 1;
}
