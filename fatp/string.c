#include <stdio.h>
#include <stdlib.h>

#include "string.h"

/*** Resizable string functions ***********************************************/

void _stc_string_push_vfmt(char **self, const char *fmt, va_list ap)
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
        stc_string_len_unsafe(*self) +=
            vsnprintf(*self + len, stc_string_cap_unsafe(*self) - len, fmt, aq);
    }
    va_end(aq);
}

void _stc_strint_push_fmt(char **self, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _stc_string_push_vfmt(self, fmt, ap);
    va_end(ap);
}

/*** String slice functions ***************************************************/

char *stc_str_from_vfmt(const char *fmt, va_list ap)
{
#define STC_STR_INIT_BUFSIZE 256
    StcSliceHeader *slice;
    va_list         aq;

    va_copy(aq, ap); /* need to make copy of ap in case retry is needed */
    slice =
        malloc(STC_STR_INIT_BUFSIZE * sizeof(char) + sizeof(StcSliceHeader));
    slice->len = vsnprintf((char *) (slice + 1), STC_STR_INIT_BUFSIZE, fmt, ap);

    /* check if buffer was too small and retry with new size if so */
    if (slice->len > STC_STR_INIT_BUFSIZE) {
        free(slice);
        slice      = malloc(slice->len * sizeof(char) + sizeof(StcSliceHeader));
        slice->len = vsnprintf((char *) (slice + 1), slice->len, fmt, aq);
    }
    va_end(aq);

    return (char *) (slice + 1);
#undef STC_STR_INIT_BUFSIZE
}

char *stc_str_from_fmt(const char *fmt, ...)
{
    char   *str;
    va_list ap;

    va_start(ap, fmt);
    str = stc_str_from_vfmt(fmt, ap);
    va_end(ap);

    return str;
}
