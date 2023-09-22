#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "string_view.h"

StcStringView stc_sv_from_parts(STC_SV_CONST char *str, size_t len)
{
    StcStringView sv;
    sv.str = str;
    sv.len = len;
    return sv;
}

StcStringView stc_sv_trim_left(StcStringView sv)
{
    for (; sv.len > 0 && isspace(*sv.str); sv.str++, sv.len--)
        ;

    return sv;
}

StcStringView stc_sv_trim_right(StcStringView sv)
{
    for (; sv.len > 0 && isspace(sv.str[sv.len - 1]); sv.len--)
        ;

    return sv;
}

StcStringView stc_sv_take_left_while(StcStringView sv, int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < sv.len && predicate(sv.str[i]); i++)
        ;

    return stc_sv_from_parts(sv.str, i);
}

StcStringView stc_sv_chop_left_while(StcStringView *sv, int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < sv->len && predicate(sv->str[i]); i++)
        ;

    return stc_sv_chop_left(sv, i);
}

StcStringView stc_sv_chop_by_delim(StcStringView *sv, char delim)
{
    size_t        i;
    StcStringView left;
    for (i = 0; i < sv->len && sv->str[i] != delim; i++)
        ;

    left = stc_sv_from_parts(sv->str, i);
    if (i < sv->len) i++;
    sv->len -= i;
    sv->str += i;

    return left;
}

StcStringView stc_sv_chop_by_sv(StcStringView *sv, StcStringView delim)
{
    size_t        i;
    StcStringView left, window = stc_sv_from_parts(sv->str, delim.len);
    for (i = 0; i + delim.len <= sv->len && !stc_sv_eq(window, delim);
         i++, window.str++)
        ;

    left = stc_sv_from_parts(sv->str, i);
    if (i + delim.len > sv->len) {
        /* include last delim.len characters since we never matched delim */
        left.len += delim.len;
    }

    /* perform chop */
    sv->str += i + delim.len;
    sv->len -= i + delim.len;

    return left;
}

int stc_sv_try_chop_by_delim(StcStringView *sv, char delim, StcStringView *left)
{
    size_t i;
    for (i = 0; i < sv->len && sv->str[i] != delim; i++)
        ;

    if (i < sv->len) {
        if (left) *left = stc_sv_from_parts(sv->str, i);
        sv->len -= i + 1;
        sv->str += i + 1;
        return 1;
    }

    return 0;
}

StcStringView stc_sv_chop_left(StcStringView *sv, size_t n)
{
    StcStringView left;

    if (n > sv->len) n = sv->len;

    left     = stc_sv_from_parts(sv->str, n);
    sv->str += n;
    sv->len -= n;

    return left;
}

StcStringView stc_sv_chop_right(StcStringView *sv, size_t n)
{
    StcStringView right;

    if (n > sv->len) n = sv->len;

    right    = stc_sv_from_parts(sv->str + sv->len - n, n);
    sv->len -= n;

    return right;
}

int stc_sv_index_of(StcStringView sv, char c, size_t *idx)
{
    size_t i;
    for (i = 0; i < sv.len && sv.str[i] != c; i++)
        ;

    if (i < sv.len) {
        if (idx) *idx = i;
        return 1;
    } else {
        return 0;
    }
}

int stc_sv_cmp(StcStringView a, StcStringView b)
{
    size_t i, min_len = a.len < b.len ? a.len : b.len;

    for (i = 0; i < min_len && a.str[i] == b.str[i]; i++)
        ;

    if (i < min_len) return a.str[i] - b.str[i];

    return a.len < b.len ? -1 : (a.len > b.len ? 1 : 0);
}

int stc_sv_eq(StcStringView a, StcStringView b)
{
    if (a.len != b.len) {
        return 0;
    } else {
        return memcmp(a.str, b.str, a.len) == 0;
    }
}

int stc_sv_eq_ignorecase(StcStringView a, StcStringView b)
{
    size_t i;

#define STC_SV_LOWER(c) ('A' <= (c) && (c) <= 'Z' ? (c) + 32 : (c))

    if (a.len != b.len) return 0;
    for (i = 0; i < a.len; i++)
        if (STC_SV_LOWER(a.str[i]) != STC_SV_LOWER(b.str[i])) return 0;

#undef STC_SV_LOWER

    return 1;
}

int stc_sv_starts_with(StcStringView sv, StcStringView prefix)
{
    if (prefix.len <= sv.len)
        return stc_sv_eq(prefix, stc_sv_from_parts(sv.str, prefix.len));

    return 0;
}

int stc_sv_ends_with(StcStringView sv, StcStringView suffix)
{
    if (suffix.len <= sv.len)
        return stc_sv_eq(suffix, stc_sv_from_parts(sv.str + sv.len - suffix.len,
                                                   suffix.len));

    return 0;
}

#define STC_SV_PROCESS_DIGIT(n, c) (n) = 10 * (n) + (c) - '0'

size_t stc_sv_to_int(StcStringView sv)
{
    size_t i, num;

    for (i = 0, num = 0; i < sv.len && isdigit(sv.str[i]); i++)
        STC_SV_PROCESS_DIGIT(num, sv.str[i]);

    return num;
}

size_t stc_sv_chop_int(StcStringView *sv)
{
    size_t num;

    for (num = 0; sv->len > 0 && isdigit(*sv->str); sv->len--, sv->str++)
        STC_SV_PROCESS_DIGIT(num, *sv->str);

    return num;
}

#undef STC_SV_PROCESS_DIGIT
