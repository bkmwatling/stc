#include <ctype.h>
#include <stdio.h>

#include <stc/fatp/str_view.h>

StcStrView stc_sv_trim_left(StcStrView self)
{
    for (; self.len > 0 && isspace(stc_sv_at(self, 0)); self.str++, self.len--);

    return self;
}

StcStrView stc_sv_trim_right(StcStrView self)
{
    for (; self.len > 0 && isspace(stc_sv_at(self, self.len - 1)); self.len--);

    return self;
}

StcStrView stc_sv_take_left_while(StcStrView self, int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < self.len && predicate(stc_sv_at(self, i)); i++);

    return stc_sv_from_parts(self.str, i);
}

StcStrView stc_sv_split_left_while(StcStrView *self, int (*predicate)(char))
{
    size_t i;

    for (i = 0; i < self->len && predicate(stc_sv_at(*self, i)); i++);

    return stc_sv_split_left(self, i);
}

StcStrView stc_sv_split_by_delim(StcStrView *self, char delim)
{
    size_t     i;
    StcStrView left;
    for (i = 0; i < self->len && stc_sv_at(*self, i) != delim; i++);

    left = stc_sv_from_parts(self->str, i);
    if (i < self->len) i++;
    self->len -= i;
    self->str += i;

    return left;
}

StcStrView stc_sv_split_by_sv(StcStrView *self, StcStrView delim)
{
    size_t     i;
    StcStrView left, window = stc_sv_from_parts(self->str, delim.len);
    for (i = delim.len; i <= self->len && !stc_sv_eq(window, delim);
         i++, window.str++);

    left = stc_sv_from_parts(self->str, i - delim.len);
    if (i > self->len)
        /* set left.len to self->len and update i to be self->len for split
         * since we never matched delim */
        i = left.len = self->len;

    /* perform split */
    self->str += i;
    self->len -= i;

    return left;
}

int stc_sv_try_split_by_delim(StcStrView *self, char delim, StcStrView *left)
{
    size_t i;
    for (i = 0; i < self->len && stc_sv_at(*self, i) != delim; i++);

    if (i < self->len) {
        if (left) *left = stc_sv_from_parts(self->str, i);
        self->len -= i + 1;
        self->str += i + 1;
        return 1;
    }

    return 0;
}

StcStrView stc_sv_split_left(StcStrView *self, size_t n)
{
    StcStrView left;

    if (n > self->len) n = self->len;

    left       = stc_sv_from_parts(self->str, n);
    self->str += n;
    self->len -= n;

    return left;
}

StcStrView stc_sv_split_right(StcStrView *self, size_t n)
{
    StcStrView right;

    if (n > self->len) n = self->len;

    right      = stc_sv_from_parts(&stc_sv_at(*self, self->len - n), n);
    self->len -= n;

    return right;
}

int stc_sv_index_of(StcStrView self, char c, size_t *idx)
{
    size_t i;
    for (i = 0; i < self.len && stc_sv_at(self, i) != c; i++);

    if (i < self.len) {
        if (idx) *idx = i;
        return 1;
    } else {
        return 0;
    }
}

int stc_sv_cmp(StcStrView a, StcStrView b)
{
    size_t i, min_len = a.len < b.len ? a.len : b.len;

    for (i = 0; i < min_len && stc_sv_at(a, i) == stc_sv_at(b, i); i++);

    if (i < min_len) return stc_sv_at(a, i) - stc_sv_at(b, i);

    return a.len < b.len ? -1 : (a.len > b.len ? 1 : 0);
}

int stc_sv_eq(StcStrView a, StcStrView b)
{
    if (a.len != b.len)
        return 0;
    else
        return memcmp(a.str, b.str, a.len) == 0;
}

int stc_sv_eq_ignorecase(StcStrView a, StcStrView b)
{
    size_t i;

#define STC_SV_LOWER(c) ('A' <= (c) && (c) <= 'Z' ? (c) + 32 : (c))

    if (a.len != b.len) return 0;
    for (i = 0; i < a.len; i++)
        if (STC_SV_LOWER(stc_sv_at(a, i)) != STC_SV_LOWER(stc_sv_at(b, i)))
            return 0;

#undef STC_SV_LOWER

    return 1;
}

int stc_sv_starts_with(StcStrView self, StcStrView prefix)
{
    if (prefix.len <= self.len)
        return stc_sv_eq(prefix, stc_sv_from_parts(self.str, prefix.len));

    return 0;
}

int stc_sv_ends_with(StcStrView self, StcStrView suffix)
{
    if (suffix.len <= self.len)
        return stc_sv_eq(
            suffix, stc_sv_from_parts(&stc_sv_at(self, self.len - suffix.len),
                                      suffix.len));

    return 0;
}

#define STC_SV_PROCESS_DIGIT(n, c) (n) = 10 * (n) + (c) - '0'

size_t stc_sv_to_int(StcStrView self)
{
    size_t i, num;

    for (i = 0, num = 0; i < self.len && isdigit(stc_sv_at(self, i)); i++)
        STC_SV_PROCESS_DIGIT(num, stc_sv_at(self, i));

    return num;
}

size_t stc_sv_split_int(StcStrView *self)
{
    size_t num;

    for (num = 0; self->len > 0 && isdigit(stc_sv_at(*self, 0));
         self->len--, self->str++)
        STC_SV_PROCESS_DIGIT(num, stc_sv_at(*self, 0));

    return num;
}

#undef STC_SV_PROCESS_DIGIT
