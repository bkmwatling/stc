#include <stdio.h>
#include <stdlib.h>

#include <stc/fatp/string.h>

/* --- Resizable string functions ------------------------------------------- */

void _stc_string_push_vfmt(StcString *self, const char *fmt, va_list ap)
{
    size_t  cap, len, n;
    va_list aq;

    va_copy(aq, ap); /* need to make copy of ap in case retry is needed */
    len = self->len;
    cap = self->cap;
    n   = vsnprintf(&stc_string_at(*self, len), cap - len, fmt, ap);

    /* check if buffer was too small and retry with new size if so */
    if (n > cap - len) {
        stc_string_reserve(self, n);
        if (self->cap - len < n)
            n = self->cap - len; /* cap size incase not enough space */
        vsnprintf(&stc_string_at(*self, len), n, fmt, aq);
    }
    va_end(aq);
    self->len += n;
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
    char   *s;
    size_t  len;
    va_list aq;

    va_copy(aq, ap); /* need to make copy of ap in case retry is needed */
    /* to comply with all C standards (SUSv2 specifically) we can't give a size
     * of 0 to vsnprintf and as such we must allocate string of length 1 */
    s   = malloc(sizeof(char));
    len = vsnprintf(s, 1, fmt, ap);

    /* check if buffer was too small (should be) and retry with new len if so */
    if (len > 1) {
        s   = realloc(s, sizeof(char) * len);
        len = vsnprintf(s, len, fmt, aq);
    }
    va_end(aq);

    return stc_str_from_parts(s, len);
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
