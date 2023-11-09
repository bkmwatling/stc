#include <stdlib.h>
#include <string.h>

#include "utf.h"

/*** Helper functions *********************************************************/

static int _stc_utf8_nbytes(const char *codepoint, size_t min_len)
{
    int nbytes = 0;

    if (min_len >= 1 && STC_UTF8_IS_SINGLE(codepoint)) {
        /* is valid single byte (i.e. 0xxx xxxx) */
        nbytes = 1;
    } else if (min_len >= 2 && STC_UTF8_IS_DOUBLE(codepoint)) {
        /* is valid double byte (i.e. 110x xxxx and 1 continuation byte) */
        nbytes = 2;
    } else if (min_len >= 3 && STC_UTF8_IS_TRIPLE(codepoint)) {
        /* is valid triple byte (i.e. 1110 xxxx and 2 continuation bytes) */
        nbytes = 3;
    } else if (min_len >= 4 && STC_UTF8_IS_QUADRUPLE(codepoint)) {
        /* is valid quadruple byte (i.e. 1111 0xxx and 3 continuation bytes) */
        nbytes = 4;
    }

    return nbytes;
}

/*** Single UTF-8 codepoint functions *****************************************/

int stc_utf8_nbytes(const char *codepoint)
{
    int min_len = 0;

    while (min_len < 4 && codepoint[min_len++])
        ;

    return _stc_utf8_nbytes(codepoint, min_len);
}

char *stc_utf8_to_str(const char *codepoint)
{
    char  *p;
    size_t n = stc_utf8_nbytes(codepoint);

    if (n == 0) return NULL;

    p = malloc((n + 1) * sizeof(char));
    strncpy(p, codepoint, n);
    p[n] = '\0';

    return p;
}

#define STC_UTF8_CMP(a, b, alen, blen) \
    ((alen) == (blen) ? strncmp((a), (b), (alen)) : (alen) - (blen));

int stc_utf8_cmp(const char *a, const char *b)
{
    int alen = stc_utf8_nbytes(a), blen = stc_utf8_nbytes(b);

    return STC_UTF8_CMP(a, b, alen, blen);
}

int stc_utf8_try_cmp(const char *a, const char *b, int *cmp)
{
    int result = 0;
    int alen = stc_utf8_nbytes(a), blen = stc_utf8_nbytes(b);

    if (alen == 0) result++;
    if (blen == 0) result += 2;
    if (cmp && result == 0) *cmp = STC_UTF8_CMP(a, b, alen, blen);

    return result;
}

#undef STC_UTF8_CMP

/*** UTF-8 encoded strings functions ******************************************/

size_t stc_utf8_str_ncodepoints(const char *s)
{
    int    nbytes;
    size_t ncodepoints = 0;

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
    int n = stc_utf8_nbytes(s);
    return n > 0 ? s + n : NULL;
}

const char *stc_utf8_str_advance(const char **s)
{
    const char *codepoint = NULL;
    int         n         = s && *s ? stc_utf8_nbytes(*s) : 0;

    if (n > 0) {
        codepoint  = *s;
        *s        += n;
    }

    return codepoint;
}

/*** UTF-8 string view functions **********************************************/

#ifndef STC_UTF_DISABLE_SV

/*** Single UTF-8 codepoint functions for string views ************************/

int stc_utf8_nbytes_sv(StcStringView sv)
{
    return _stc_utf8_nbytes(sv.str, sv.len);
}

StcStringView stc_utf8_to_sv(const char *codepoint)
{
    return stc_sv_from_parts(codepoint, stc_utf8_nbytes(codepoint));
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

/*** UTF-8 encoded strings functions for string views *************************/

size_t stc_utf8_sv_ncodepoints(StcStringView sv)
{
    int    nbytes;
    size_t ncodepoints = 0;

    while (sv.len > 0) {
        if ((nbytes = stc_utf8_nbytes(sv.str)) && sv.len >= (size_t) nbytes) {
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
    int n = stc_utf8_nbytes_sv(sv);

    if (n > 0) {
        sv.str += n;
        sv.len -= n;
    } else {
        sv.len = 0;
    }

    return sv;
}

StcStringView stc_utf8_sv_advance(StcStringView *sv)
{
    int           n            = sv ? stc_utf8_nbytes_sv(*sv) : 0;
    StcStringView codepoint_sv = { n, NULL };

    if (n > 0) {
        codepoint_sv.str  = sv->str;
        sv->len          -= n;
        sv->str          += n;
    }

    return codepoint_sv;
}

#endif /* STC_UTF_DISABLE_SV */
