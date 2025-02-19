#ifndef STC_STR_VIEW_H
#define STC_STR_VIEW_H

#include <stddef.h>
#include <string.h>

#include <stc/fatp/slice.h>

/** String view definition which simply views a string over a length */
typedef struct {
    size_t      len;
    const char *str;
} StcStrView;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SV_ENABLE_SHORT_NAMES)
typedef StcStrView StrView;

#    define SV_FMT STC_SV_FMT
#    define SV_ARG STC_SV_ARG

#    define sv_from_parts stc_sv_from_parts
#    define sv_from_range stc_sv_from_range
#    define sv_from_cstr  stc_sv_from_cstr
#    define sv            stc_sv
#    define sv_from_str   stc_sv_from_str

#    define sv_at    stc_sv_at
#    define sv_first stc_sv_first
#    define sv_last  stc_sv_last

#    define sv_subview       stc_sv_subview
#    define sv_subview_from  stc_sv_subview_from
#    define sv_subview_until stc_sv_subview_until

#    define sv_trim               stc_sv_trim
#    define sv_trim_left          stc_sv_trim_left
#    define sv_trim_right         stc_sv_trim_right
#    define sv_take_left_while    stc_sv_take_left_while
#    define sv_split_left_while   stc_sv_split_left_while
#    define sv_split_by_delim     stc_sv_split_by_delim
#    define sv_split_by_sv        stc_sv_split_by_sv
#    define sv_try_split_by_delim stc_sv_try_split_by_delim
#    define sv_split_left         stc_sv_split_left
#    define sv_split_right        stc_sv_split_right
#    define sv_index_of           stc_sv_index_of
#    define sv_cmp                stc_sv_cmp
#    define sv_eq                 stc_sv_eq
#    define sv_eq_ignorecase      stc_sv_eq_ignorecase
#    define sv_starts_with        stc_sv_starts_with
#    define sv_ends_with          stc_sv_ends_with
#    define sv_to_int             stc_sv_to_int
#    define sv_split_int          stc_sv_split_int
#endif /* STC_SV_ENABLE_SHORT_NAMES */

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_STR_ENABLE_SHORT_NAMES)
#    define str_view stc_str_view

#    define str_subview       stc_str_subview
#    define str_subview_from  stc_str_subview_from
#    define str_subview_until stc_str_subview_until
#endif /* STC_STR_ENABLE_SHORT_NAMES */

/**
 * printf macros for StcStrView
 * USAGE:
 *   StcStrView name = ...;
 *   printf("Name: " STC_SV_FMT "\n", STC_SV_ARG(name));
 */
#define STC_SV_FMT     "%.*s"
#define STC_SV_ARG(sv) (int) (sv).len, (sv).str

/**
 * Create a string view from a character pointer and length.
 *
 * @param[in] s   the string (character pointer) to view
 * @param[in] len the length of the string view
 *
 * @return a string view of the given string over length of len
 */
#define stc_sv_from_parts(s, len) ((StcStrView) { (len), (s) })

/**
 * Create a string view of the range between two strings (character pointers).
 *
 * NOTE: The created string view is non-inclusive of the end of the range.
 *
 * @param[in] start the start (pointer) of the string range
 * @param[in] end   the end (pointer) of the string range
 *
 * @return a string view of the string defined by the range
 */
#define stc_sv_from_range(start, end)                                         \
    __extension__({                                                           \
        __auto_type _STC_MACRO_VAR(_stc_sv_from_range_start_) = (start);      \
        stc_sv_from_parts(_STC_MACRO_VAR(_stc_sv_from_range_start_),          \
                          (end) - _STC_MACRO_VAR(_stc_sv_from_range_start_)); \
    })

/**
 * Create a string view from a C (null terminated) string by calling strlen for
 * automatically.
 *
 * @param[in] cstr the C string to view
 *
 * @return a string view of the C string
 */
#define stc_sv_from_cstr(cstr)                                              \
    __extension__({                                                         \
        __auto_type = _STC_MACRO_VAR(_stc_sv_from_cstr_cstr_) = (cstr);     \
        stc_sv_from_parts(_STC_MACRO_VAR(_stc_sv_from_cstr_cstr_),          \
                          strlen(_STC_MACRO_VAR(_stc_sv_from_cstr_cstr_))); \
    })

/**
 * Create a string view from a string literal (similar to C++ sv keyword for
 * literals).
 *
 * @param[in] lit the string literal to view
 *
 * @return a string view of the string literal
 */
#define stc_sv(lit) stc_sv_from_parts(lit, sizeof(lit) - 1)

/**
 * Create a string view from a string slice.
 *
 * @param[in] str the string slice to view
 *
 * @return a string view of the string slice
 */
#define stc_sv_from_str(str)                                                   \
    __extension__({                                                            \
        __auto_type _STC_MACRO_VAR(_stc_sv_from_str_str_) = (str);             \
        stc_sv_from_parts(_STC_MACRO_VAR(_stc_sv_from_str_str_),               \
                          stc_str_len(_STC_MACRO_VAR(_stc_sv_from_str_str_))); \
    })

/**
 * Create a string view from a string slice.
 *
 * @param[in] str the string slice to view
 *
 * @return a string view of the string slice
 */
#define stc_str_view stc_sv_from_str

/**
 * Get the character at the specified index of a string view.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] sv the string view to get the indexed character from
 * @param[in] i  the index of the character to retrieve
 *
 * @return the indexed character from the string view
 */
#define stc_sv_at(sv, i) (sv).str[i]

/**
 * Get the first element from a string view.
 *
 * @param[in] sv the string view to retrieve the first element of
 *
 * @return the first element of the string view
 */
#define stc_sv_first(sv) stc_sv_at(sv, 0)

/**
 * Get the last element from a string view.
 *
 * @param[in] sv the string view to retrieve the last element of
 *
 * @return the last element of the string view
 */
#define stc_sv_last(sv)                                     \
    ({                                                      \
        StcStrView _STC_MACRO_VAR(_stc_sv_last_sv_) = (sv); \
        stc_sv_at(_STC_MACRO_VAR(_stc_sv_last_sv_),         \
                  _STC_MACRO_VAR(_stc_sv_last_sv_).len);    \
    })

/**
 * Create a substring view from a string view from the starting index until the
 * ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] sv    the string view to create the substring view from
 * @param[in] start the start index of the substring view
 * @param[in] end   the end index of the substring view
 *
 * @return a substring view over the defined range
 */
#define stc_sv_subview(sv, start, end)                              \
    __extension__({                                                 \
        StcStrView _STC_MACRO_VAR(_stc_sv_subview_sv_) = (sv);      \
        stc_sv_from_range(                                          \
            &stc_sv_at(_STC_MACRO_VAR(_stc_sv_subview_sv_), start), \
            &stc_sv_at(_STC_MACRO_VAR(_stc_sv_subview_sv_), end));  \
    })

/**
 * Create a substring view from a string view from the starting index until the
 * end of the string view.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] sv    the string view to create the substring view from
 * @param[in] start the start index of the substring view
 *
 * @return a substring view from the starting index until the end of the string
 *         view
 */
#define stc_sv_subview_from(sv, start)                                  \
    __extension__({                                                     \
        StcStrView _STC_MACRO_VAR(_stc_sv_subview_from_sv_) = (sv);     \
        stc_sv_subview(_STC_MACRO_VAR(_stc_sv_subview_from_sv_), start, \
                       _STC_MACRO_VAR(_stc_sv_subview_from_sv_).len);   \
    })

/**
 * Create a substring view from a string view from the start of the string view
 * until the ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] sv  the string view to create the substring view from
 * @param[in] end the end index of the substring view
 *
 * @return a substring view from the start of the string view until the ending
 *         index (non-inclusive)
 */
#define stc_sv_subview_until(sv, end) stc_sv_subview(sv, 0, end)

/**
 * Create a substring view from a string slice from the starting index until the
 * ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] str   the string slice to create the substring view from
 * @param[in] start the start index of the substring view
 * @param[in] end   the end index of the substring view
 *
 * @return a substring view over the defined range
 */
#define stc_str_subview(str, start, end)                                 \
    __extension__({                                                      \
        __auto_type _STC_MACRO_VAR(_stc_str_subview_str_) = (str);       \
        stc_sv_from_range(&_STC_MACRO_VAR(_stc_str_subview_str_)[start], \
                          &_STC_MACRO_VAR(_stc_str_subview_str_)[end]);  \
    })

/**
 * Create a substring view from a string slice from the starting index until the
 * end of the string slice.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] str   the string slice to create the substring view from
 * @param[in] start the start index of the substring view
 *
 * @return a substring view from the starting index until the end of the string
 *         slice
 */
#define stc_str_subview_from(str, start)                                \
    __extension__({                                                     \
        __auto_type _STC_MACRO_VAR(_stc_str_subview_from_str_) = (str); \
        stc_str_subview(                                                \
            _STC_MACRO_VAR(_stc_str_subview_from_str_), start,          \
            stc_str_len(_STC_MACRO_VAR(_stc_str_subview_from_str_)));   \
    })

/**
 * Create a substring view from a string slice from the start of the string
 * slice until the ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] str the string slice to create the substring view from
 * @param[in] end the end index of the substring view
 *
 * @return a substring view from the start of the string slice until the ending
 *         index (non-inclusive)
 */
#define stc_str_subview_until(str, end) stc_str_subview(str, 0, end)

/**
 * Trim the string view from both sides (skipping over spaces), and return the
 * trimmed string view.
 *
 * @param[in] sv the string view to trim
 *
 * @return the trimmed string view
 */
#define stc_sv_trim(sv) stc_sv_trim_left(stc_sv_trim_right((sv)))

/**
 * Trim the string view from the left (skipping over spaces), and return the
 * trimmed string view.
 *
 * @param[in] self the string view to trim from the left
 *
 * @return the trimmed string view
 */
StcStrView stc_sv_trim_left(StcStrView self);

/**
 * Trim the string view from the right (skipping over spaces), and return the
 * trimmed string view.
 *
 * @param[in] self the string view to trim from the right
 *
 * @return the trimmed string view
 */
StcStrView stc_sv_trim_right(StcStrView self);

/**
 * Take the left side (prefix) of the string view such that the returned string
 * view's characters all match the predicate.
 *
 * The returned string view is a maximal prefix of the given string view with
 * all characters matching the predicate.
 *
 * @param[in] self      the string view to take from
 * @param[in] predicate the predicate function to test the characters of the
 *                      string view
 *
 * @return a string view of the maximal prefix of self with all characters
 *         matching the predicate
 */
StcStrView stc_sv_take_left_while(StcStrView self, int (*predicate)(char));

/**
 * Split the string view self from the left and return the left side (prefix) of
 * self such that the returned string view's characters all match the predicate.
 *
 * The returned string view is a maximal prefix of the self with all characters
 * matching the predicate. Then self is updated to point to the string after the
 * returned prefix. If the all the characters of self match the predicate then
 * self will be set to have length 0 and point to the character after the end of
 * the string it used to view. In this case the returned string view will be a
 * copy of what self was.
 *
 * @param[in,out] self      the pointer to the string view to split
 * @param[in]     predicate the predicate function to test the characters of the
 *                          string view
 *
 * @return a string view of the maximal prefix of self with all characters
 *         matching the predicate
 */
StcStrView stc_sv_split_left_while(StcStrView *self, int (*predicate)(char));

/**
 * Split the string view self at the delimeter and return a string view of the
 * self from the left of the delimeter and update self to view the string from
 * the right of the delimeter.
 *
 * If the delimeter is not in self, then self will be set to have length 0 and
 * point to the end of the string is used to view. In this case the returned
 * string view will be a copy of what self was.
 *
 * @param[in,out] self  the pointer to the string view to split
 * @param[in]     delim the character delimeter to split the string view at
 *
 * @return a string view of the string to the left of the delimeter
 */
StcStrView stc_sv_split_by_delim(StcStrView *self, char delim);

/**
 * Split the string view self at the delimeter and return a string view of the
 * self from the left of the delimeter and update self to view the string from
 * the right of the delimeter.
 *
 * If the delimeter is not in self, then self will be set to have length 0 and
 * point to the end of the string is used to view. In this case the returned
 * string view will be a copy of what self was.
 *
 * @param[in,out] self  the pointer to the string view to split
 * @param[in]     delim the string view delimeter to split the string view at
 *
 * @return a string view of the string to the left of the delimeter
 */
StcStrView stc_sv_split_by_sv(StcStrView *self, StcStrView delim);

/**
 * Try to split the string view self at the delimeter and return whether or not
 * the delimeter was found in self.
 *
 * If the delimeter is found in self, then a string view of self from the left
 * of the delimeter is returned through the second string view pointer and self
 * is updated to view the string from the right of the delimeter. If the
 * delimeter is not in self, then nothing will happen and self will remain
 * unchanged. In this case the second string view pointer is also not modified.
 *
 * NOTE: if the second string view pointer is NULL, setting its memory is not
 * attempted, and so it can be set to NULL if you don't care about string on the
 * left side of the delimeter.
 *
 * @param[in,out] self  the pointer to the string view to split
 * @param[in]     delim the string view delimeter to split the string view at
 * @param[out]    left  the pointer to the string view to store the string on
 *                      the left of the delimiter if found and is not NULL
 *
 * @return whether the delimeter was found and thus if the string views where
 *         modified
 */
int stc_sv_try_split_by_delim(StcStrView *self, char delim, StcStrView *left);

/**
 * Split off the first n characters from the string view and return a string
 * view to the split off characters whilst updating the given string view to
 * start after the last split off character.
 *
 * @param[in,out] self the pointer to the string view to split
 * @param[in]     n    the number of characters to split from the left
 *
 * @return a string view to the characters that were split off
 */
StcStrView stc_sv_split_left(StcStrView *self, size_t n);

/**
 * Split off the last n characters from the string view and return a string view
 * to the split off characters whilst updating the given string view to end
 * before the first split off character.
 *
 * @param[in,out] self the pointer to the string view to split
 * @param[in]     n    the number of characters to split from the right
 *
 * @return a string view to the characters that were split off
 */
StcStrView stc_sv_split_right(StcStrView *self, size_t n);

/**
 * Find the index of the character c in the string view if it is contained in
 * the string view.
 *
 * @param[in]  self the string view to find the index of c in
 * @param[in]  c    the character to find the index of
 * @param[out] idx  the pointer to save the index of c in only if found
 *
 * @return a non-zero value if the character was found in the string view;
 *         else 0 if the character was not found in the string view
 */
int stc_sv_index_of(StcStrView self, char c, size_t *idx);

/**
 * Compare two string views.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_sv_cmp(StcStrView a, StcStrView b);

/**
 * Determine if two string views are equal.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a non-zero value if a and b are equal; else 0 if they are not equal
 */
int stc_sv_eq(StcStrView a, StcStrView b);

/**
 * Determine if two string views are equal in a case-insesitive manner.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a non-zero value if a and b are equal whilst ignoring case; else 0 if
 *         they are not equal whilst ignoring case
 */
int stc_sv_eq_ignorecase(StcStrView a, StcStrView b);

/**
 * Check if a string view is a prefix of the other string view.
 *
 * @param[in] self   the string view to check the prefix of
 * @param[in] prefix the string view to test if it is a prefix of self
 *
 * @return a non-zero value of the prefix string view is a prefix of self;
 *         else 0 if it is not
 */
int stc_sv_starts_with(StcStrView self, StcStrView prefix);

/**
 * Check if a string view is a suffix of the other string view.
 *
 * @param[in] self   the string view to check the suffix of
 * @param[in] suffix the string view to test if it is a suffix of self
 *
 * @return a non-zero value of the suffix string view is a suffix of self;
 *         else 0 if it is not
 */
int stc_sv_ends_with(StcStrView self, StcStrView suffix);

/**
 * Convert the string view into an unsigned integer, ignoring any junk after the
 * last contiguous digit.
 *
 * If the first character is not a digit then zero is returned.
 *
 * @param[in] self the string view to parse the unsigned integer from
 *
 * @return the unsigned integer parsed from the string view
 */
size_t stc_sv_to_int(StcStrView self);

/**
 * Convert the string view into an unsigned integer, ignoring any junk after
 * the last contiguous digit.
 *
 * The string view is changed to view from the character after the last
 * contiguous digit. If the first character is not a digit then zero is returned
 * and the string view is not modified. If there are no characters after the
 * last contiguous digit, then the string view will have length 0 and point to
 * the character after the end of the string it used to view.
 *
 * @param[in] self the pointer to the string view to parse the unsigned integer
 *                 from
 *
 * @return the unsigned integer parsed from the string view
 */
size_t stc_sv_split_int(StcStrView *self);

#endif /* STC_STR_VIEW_H */
