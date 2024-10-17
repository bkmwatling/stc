#include <stdlib.h>
#include <string.h>

#include <stc/fatp/slice.h>

void stc_slice_free(void *slice)
{
    if (slice) free(stc_slice_header(slice));
}

void *_stc_slice_from_parts(const void *p, size_t size, size_t len)
{
    StcSliceHeader *slice;

    if (size == 0 || len == 0) return NULL;
    slice      = malloc(size * len + sizeof(StcSliceHeader));
    slice->len = len;
    if (p) memcpy(slice + 1, p, size * len);

    return slice + 1;
}
