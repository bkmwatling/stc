#include <stdlib.h>
#include <string.h>

#include <stc/fatp/slice.h>

void stc_slice_free(StcSlice(void) slice)
{
    if (slice) free(stc_slice_header(slice));
}

StcSlice(void) _stc_slice_from_parts(const void *p, size_t size, size_t len)
{
    StcSliceHeader *slice;

    if (size == 0 || len == 0) return NULL;
    slice      = malloc(sizeof(*slice) + size * len);
    slice->len = len;
    if (p) memcpy(slice + 1, p, size * len);

    return slice + 1;
}
