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

StcStringView stc_sv_trim_left(StcStringView self)
{
    for (; self.len > 0 && isspace(*self.str); self.str++, self.len--)
        ;

    return self;
}

StcStringView stc_sv_trim_right(StcStringView self)
{
    for (; self.len > 0 && isspace(self.str[self.len - 1]); self.len--)
        ;

    return self;
}

StcStringView stc_sv_take_left_while(StcStringView self, int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < self.len && predicate(self.str[i]); i++)
        ;

    return stc_sv_from_parts(self.str, i);
}

StcStringView stc_sv_chop_left_while(StcStringView *self,
                                     int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < self->len && predicate(self->str[i]); i++)
        ;

    return stc_sv_chop_left(self, i);
}

StcStringView stc_sv_chop_by_delim(StcStringView *self, char delim)
{
    size_t        i;
    StcStringView left;
    for (i = 0; i < self->len && self->str[i] != delim; i++)
        ;

    left = stc_sv_from_parts(self->str, i);
    if (i < self->len) i++;
    self->len -= i;
    self->str += i;

    return left;
}

StcStringView stc_sv_chop_by_sv(StcStringView *self, StcStringView delim)
{
    size_t        i;
    StcStringView left, window = stc_sv_from_parts(self->str, delim.len);
    for (i = 0; i + delim.len <= self->len && !stc_sv_eq(window, delim);
         i++, window.str++)
        ;

    left = stc_sv_from_parts(self->str, i);
    if (i + delim.len > self->len) {
        /* include last delim.len characters since we never matched delim */
        left.len += delim.len;
    }

    /* perform chop */
    self->str += i + delim.len;
    self->len -= i + delim.len;

    return left;
}

int stc_sv_try_chop_by_delim(StcStringView *self,
                             char           delim,
                             StcStringView *left)
{
    size_t i;
    for (i = 0; i < self->len && self->str[i] != delim; i++)
        ;

    if (i < self->len) {
        if (left) *left = stc_sv_from_parts(self->str, i);
        self->len -= i + 1;
        self->str += i + 1;
        return 1;
    }

    return 0;
}

StcStringView stc_sv_chop_left(StcStringView *self, size_t n)
{
    StcStringView left;

    if (n > self->len) n = self->len;

    left       = stc_sv_from_parts(self->str, n);
    self->str += n;
    self->len -= n;

    return left;
}

StcStringView stc_sv_chop_right(StcStringView *self, size_t n)
{
    StcStringView right;

    if (n > self->len) n = self->len;

    right      = stc_sv_from_parts(self->str + self->len - n, n);
    self->len -= n;

    return right;
}

int stc_sv_index_of(StcStringView self, char c, size_t *idx)
{
    size_t i;
    for (i = 0; i < self.len && self.str[i] != c; i++)
        ;

    if (i < self.len) {
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

int stc_sv_starts_with(StcStringView self, StcStringView prefix)
{
    if (prefix.len <= self.len)
        return stc_sv_eq(prefix, stc_sv_from_parts(self.str, prefix.len));

    return 0;
}

int stc_sv_ends_with(StcStringView self, StcStringView suffix)
{
    if (suffix.len <= self.len)
        return stc_sv_eq(
            suffix,
            stc_sv_from_parts(self.str + self.len - suffix.len, suffix.len));

    return 0;
}

#define STC_SV_PROCESS_DIGIT(n, c) (n) = 10 * (n) + (c) - '0'

size_t stc_sv_to_int(StcStringView self)
{
    size_t i, num;

    for (i = 0, num = 0; i < self.len && isdigit(self.str[i]); i++)
        STC_SV_PROCESS_DIGIT(num, self.str[i]);

    return num;
}

size_t stc_sv_chop_int(StcStringView *self)
{
    size_t num;

    for (num = 0; self->len > 0 && isdigit(*self->str);
         self->len--, self->str++)
        STC_SV_PROCESS_DIGIT(num, *self->str);

    return num;
}

#undef STC_SV_PROCESS_DIGIT
