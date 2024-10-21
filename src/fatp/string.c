#include <stdio.h>
#include <stdlib.h>

#include <stc/fatp/string.h>

/* --- Resizable string functions ------------------------------------------- */

void _stc_string_push_vfmt(StcString *self, const char *fmt, va_list ap)
{
    size_t  cap, len, n;
    va_list aq;

    va_copy(aq, ap); /* need to make copy of ap in case retry is needed */
    cap = stc_string_cap_unsafe(*self);
    len = stc_string_len_unsafe(*self);
    n   = vsnprintf(*self + len, cap - len, fmt, ap);

    /* check if buffer was too small and retry with new size if so */
    if (n > cap - len) {
        stc_string_reserve(*self, n);
        n = vsnprintf(*self + len, stc_string_cap_unsafe(self) - len, fmt, aq);
    }
    va_end(aq);
    stc_string_len_unsafe(self) += n;
}

void _stc_string_push_fmt(StcString *self, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _stc_string_push_vfmt(self, fmt, ap);
    va_end(ap);
}

/* --- String slice functions ----------------------------------------------- */

StcStr stc_str_from_vfmt(const char *fmt, va_list ap)
{
    StcSliceHeader *slice;
    size_t          len;
    va_list         aq;

    va_copy(aq, ap); /* need to make copy of ap in case retry is needed */
    /* to comply with all C standards (SUSv2 specifically) we can't give a size
     * of 0 to vsnprintf and as such we must allocate string of length 1 */
    slice = malloc(sizeof(*slice) + sizeof(char));
    len   = vsnprintf((char *) (slice + 1), 1, fmt, ap);

    /* check if buffer was too small (should be) and retry with new len if so */
    if (len > 1) {
        slice = realloc(slice, sizeof(*slice) + len * sizeof(char));
        len   = vsnprintf((char *) (slice + 1), len, fmt, aq);
    }
    va_end(aq);
    slice->len = len;

    return (char *) (slice + 1);
}

StcStr stc_str_from_fmt(const char *fmt, ...)
{
    StcStr  str;
    va_list ap;

    va_start(ap, fmt);
    str = stc_str_from_vfmt(fmt, ap);
    va_end(ap);

    return str;
}
