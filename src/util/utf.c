#include <stdlib.h>
#include <string.h>

#include "utf.h"

/* --- Single UTF-8 "character" functions ----------------------------------- */

unsigned int stc_utf8_nbytes(const char *ch)
{
    unsigned int nbytes = 0;

    if (STC_UTF8_IS_SINGLE(ch)) /* is valid single byte (i.e. 0xxx xxxx) */
        nbytes = 1;
    else if (STC_UTF8_IS_DOUBLE(ch))
        /* is valid double byte (i.e. 110x xxxx and 1 continuation byte) */
        nbytes = 2;
    else if (STC_UTF8_IS_TRIPLE(ch))
        /* is valid triple byte (i.e. 1110 xxxx and 2 continuation bytes) */
        nbytes = 3;
    else if (STC_UTF8_IS_QUADRUPLE(ch))
        /* is valid quadruple byte (i.e. 1111 0xxx and 3 continuation bytes) */
        nbytes = 4;

    return nbytes;
}

char *stc_utf8_clone(const char *ch)
{
    char        *p;
    unsigned int n = stc_utf8_nbytes(ch);

    if (n == 0) return NULL;

    p = malloc((n + 1) * sizeof(*p));
    strncpy(p, ch, n);
    p[n] = '\0';

    return p;
}

#define STC_UTF8_CMP(a, b, alen, blen) \
    ((alen) == (blen) ? strncmp((a), (b), (alen)) : (alen) - (blen));

int stc_utf8_cmp(const char *a, const char *b)
{
    unsigned int alen = stc_utf8_nbytes(a), blen = stc_utf8_nbytes(b);

    return STC_UTF8_CMP(a, b, alen, blen);
}

int stc_utf8_try_cmp(const char *a, const char *b, int *cmp)
{
    int          result = 0;
    unsigned int alen = stc_utf8_nbytes(a), blen = stc_utf8_nbytes(b);

    if (alen == 0) result++;
    if (blen == 0) result += 2;
    if (cmp && result == 0) *cmp = STC_UTF8_CMP(a, b, alen, blen);

    return result;
}

#undef STC_UTF8_CMP

/* --- UTF-8 encoded strings functions -------------------------------------- */

size_t stc_utf8_str_ncodepoints(const char *s)
{
    unsigned int nbytes;
    size_t       ncodepoints = 0;

    while (*s) {
        if ((nbytes = stc_utf8_nbytes(s))) {
            s += nbytes;
            ncodepoints++;
        } else {
            return 0;
        }
    }

    return ncodepoints;
}

const char *stc_utf8_str_next(const char *s)
{
    unsigned int n = stc_utf8_nbytes(s);
    return n > 0 ? s + n : NULL;
}

const char *stc_utf8_str_advance(const char **s)
{
    const char  *codepoint = NULL;
    unsigned int n         = s && *s ? stc_utf8_nbytes(*s) : 0;

    if (n > 0) {
        codepoint  = *s;
        *s        += n;
    }

    return codepoint;
}

/* --- UTF-8 string view functions ------------------------------------------ */

#if !defined(STC_DISABLE_FATP) && !defined(STC_DISABLE_SV) && \
    !defined(STC_UTF_DISABLE_SV)

/* --- Single UTF-8 "character" functions for string views ------------------ */

unsigned int stc_utf8_nbytes_sv(StcStringView sv)
{
    unsigned int nbytes = 0;

    if (sv.len >= 1 && STC_UTF8_IS_SINGLE(sv.str))
        /* is valid single byte (i.e. 0xxx xxxx) */
        nbytes = 1;
    else if (sv.len >= 2 && STC_UTF8_IS_DOUBLE(sv.str))
        /* is valid double byte (i.e. 110x xxxx and 1 continuation byte) */
        nbytes = 2;
    else if (sv.len >= 3 && STC_UTF8_IS_TRIPLE(sv.str))
        /* is valid triple byte (i.e. 1110 xxxx and 2 continuation bytes) */
        nbytes = 3;
    else if (sv.len >= 4 && STC_UTF8_IS_QUADRUPLE(sv.str))
        /* is valid quadruple byte (i.e. 1111 0xxx and 3 continuation bytes) */
        nbytes = 4;

    return nbytes;
}

StcStringView stc_utf8_to_sv(const char *ch)
{
    return stc_sv_from_parts(ch, stc_utf8_nbytes(ch));
}

int stc_utf8_cmp_sv(StcStringView a, StcStringView b)
{
    a.len = stc_utf8_nbytes_sv(a);
    b.len = stc_utf8_nbytes_sv(b);

    return stc_sv_cmp(a, b);
}

int stc_utf8_try_cmp_sv(StcStringView a, StcStringView b, int *cmp)
{
    int result = 0;

    a.len = stc_utf8_nbytes_sv(a);
    b.len = stc_utf8_nbytes_sv(b);
    if (a.len == 0) result++;
    if (b.len == 0) result += 2;
    if (cmp && result == 0) *cmp = stc_sv_cmp(a, b);

    return result;
}

/* --- UTF-8 encoded strings functions for string views --------------------- */

size_t stc_utf8_sv_ncodepoints(StcStringView sv)
{
    unsigned int nbytes;
    size_t       ncodepoints = 0;

    while (sv.len > 0) {
        if ((nbytes = stc_utf8_nbytes_sv(sv))) {
            sv.str += nbytes;
            sv.len -= nbytes;
            ncodepoints++;
        } else {
            return 0;
        }
    }

    return ncodepoints;
}

StcStringView stc_utf8_sv_next(StcStringView sv)
{
    unsigned int nbytes = stc_utf8_nbytes_sv(sv);

    if (nbytes > 0) {
        sv.str += nbytes;
        sv.len -= nbytes;
    } else {
        sv.len = 0;
    }

    return sv;
}

StcStringView stc_utf8_sv_advance(StcStringView *sv)
{
    unsigned int  nbytes       = sv ? stc_utf8_nbytes_sv(*sv) : 0;
    StcStringView codepoint_sv = { nbytes, NULL };

    if (nbytes > 0) {
        codepoint_sv.str  = sv->str;
        sv->len          -= nbytes;
        sv->str          += nbytes;
    }

    return codepoint_sv;
}

#endif /* STC_UTF_DISABLE_SV */
