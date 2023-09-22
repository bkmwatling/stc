#ifndef STC_STRING_VIEW_H
#define STC_STRING_VIEW_H

#include <stddef.h>
#include <string.h>

#ifdef STC_SV_ENABLE_SHORT_NAMES
typedef StcStringView StringView;

#    define SV_FMT STC_SV_FMT
#    define SV_ARG STC_SV_ARG

#    define sv_from_parts stc_sv_from_parts
#    define sv_from_range stc_sv_from_range
#    define sv_from_cstr  stc_sv_from_cstr
#    define sv_from_lit   stc_sv_from_lit

#    define sv_trim              stc_sv_trim
#    define sv_trim_left         stc_sv_trim_left
#    define sv_trim_right        stc_sv_trim_right
#    define sv_take_left_while   stc_sv_take_left_while
#    define sv_chop_left_while   stc_sv_chop_left_while
#    define sv_chop_by_delim     stc_sv_chop_by_delim
#    define sv_chop_by_sv        stc_sv_chop_by_sv
#    define sv_try_chop_by_delim stc_sv_try_chop_by_delim
#    define sv_chop_left         stc_sv_chop_left
#    define sv_chop_right        stc_sv_chop_right
#    define sv_index_of          stc_sv_index_of
#    define sv_cmp               stc_sv_cmp
#    define sv_eq                stc_sv_eq
#    define sv_eq_ignorecase     stc_sv_eq_ignorecase
#    define sv_starts_with       stc_sv_starts_with
#    define sv_ends_with         stc_sv_ends_with
#    define sv_to_int            stc_sv_to_int
#    define sv_chop_int          stc_sv_chop_int
#endif /* STC_SV_ENABLE_SHORT_NAMES */

#ifdef STC_SV_DISABLE_CONST
#    define STC_SV_CONST
#else
#    define STC_SV_CONST const
#endif

typedef struct {
    size_t             len;
    STC_SV_CONST char *str;
} StcStringView;

/* printf macros for StcStringView */
#define STC_SV_FMT     "%.*s"
#define STC_SV_ARG(sv) (int) (sv).len, (sv).str
/* USAGE:
 *   StcStringView name = ...;
 *   printf("Name: "STC_SV_FMT"\n", STC_SV_ARG(name)); */

#define stc_sv_from_range(start, end) \
    stc_sv_from_parts((start), (end) - (start) + 1)
#define stc_sv_from_cstr(cstr) stc_sv_from_parts((cstr), strlen((cstr)))
#define stc_sv_from_lit(s)     stc_sv_from_parts((s), sizeof(s) - 1)
#define stc_sv_trim(sv)        stc_sv_trim_right(stc_sv_trim_left(sv))

/**
 * Creates a string view from a character pointer (string) and length.
 *
 * @param[in] str the string address to view
 * @param[in] len the length of the string view
 *
 * @return a string view of the given string over length of len
 */
StcStringView stc_sv_from_parts(STC_SV_CONST char *str, size_t len);

/**
 * Trims the string view from the left (skipping over spaces), returning the
 * trimmed string view.
 *
 * @param[in] sv the string view to trim from the left
 *
 * @return the trimmed string view
 */
StcStringView stc_sv_trim_left(StcStringView sv);

/**
 * Trims the string view from the right (skipping over spaces), returning the
 * trimmed string view.
 *
 * @param[in] sv the string view to trim from the right
 *
 * @return the trimmed string view
 */
StcStringView stc_sv_trim_right(StcStringView sv);

/**
 * Takes the left side (prefix) of the string view such that the returned string
 * view's characters all match the predicate. The returned string view is a
 * maximal prefix of the given string view with all characters matching the
 * predicate.
 *
 * @param[in] sv the string view to take from
 * @param[in] predicate the predicate function to test the characters of the
 *                      string view
 *
 * @return a string view of the maximal prefix of sv with all characters
 *         matching the predicate
 */
StcStringView stc_sv_take_left_while(StcStringView sv, int (*predicate)(char));

/**
 * Chops the string view sv from the left and returns the left side (prefix) of
 * sv such that the returned string view's characters all match the predicate.
 * The returned string view is a maximal prefix of the sv with all characters
 * matching the predicate. Then sv is updated to point to the string after the
 * returned prefix.
 * If the all the characters of sv match the predicate then sv will be set to
 * have length 0 and point to the character after the end of the string it used
 * to view. In this case the returned string view will be a copy of what sv was.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] predicate the predicate function to test the characters of the
 *                      string view
 *
 * @return a string view of the maximal prefix of sv with all characters
 *         matching the predicate
 */
StcStringView stc_sv_chop_left_while(StcStringView *sv, int (*predicate)(char));

/**
 * Chops the string view sv at the delimeter and returns a string view of the
 * sv from the left of the delimeter and updates sv to view the string from the
 * right of the delimeter.
 * If the delimeter is not in sv, then sv will be set to have length 0 and point
 * to the end of the string is used to view. In this case the returned string
 * view will be a copy of what sv was.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] delim the character delimeter to chop the string view at
 *
 * @return a string view of the string to the left of the delimeter
 */
StcStringView stc_sv_chop_by_delim(StcStringView *sv, char delim);

/**
 * Chops the string view sv at the delimeter and returns a string view of the
 * sv from the left of the delimeter and updates sv to view the string from the
 * right of the delimeter.
 * If the delimeter is not in sv, then sv will be set to have length 0 and point
 * to the end of the string is used to view. In this case the returned string
 * view will be a copy of what sv was.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] delim the string view delimeter to chop the string view at
 *
 * @return a string view of the string to the left of the delimeter
 */
StcStringView stc_sv_chop_by_sv(StcStringView *sv, StcStringView delim);

/**
 * Tries to chop the string view sv at the delimeter and returns whether or not
 * the delimeter was found in sv. If the delimeter is found in sv, then a string
 * view of the sv from the left of the delimeter is returned through the second
 * string view pointer and sv is updated to view the string from the right of
 * the delimeter.
 * If the delimeter is not in sv, then nothing will happen and sv will remain
 * unchanged. In this case the second string view pointer is also not modified.
 *
 * Note: if the second string view pointer is NULL, setting its memory is not
 * attempted, and so it can be set to NULL if you don't care about string on the
 * left side of the delimeter.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] delim the string view delimeter to chop the string view at
 * @param[out] left the pointer to the string view to store the string on the
 *                  left of the delimiter if found and is not NULL
 *
 * @return whether the delimeter was found and thus if the string views where
 *         modified
 */
int stc_sv_try_chop_by_delim(StcStringView *sv,
                             char           delim,
                             StcStringView *left);

/**
 * Chops off the first n characters from the string view and returns a string
 * view to the chopped off characters whilst updating the given string view to
 * start after the last chopped off character.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] n the number of characters to chop from the left
 *
 * @return a string view to the characters that were chopped off
 */
StcStringView stc_sv_chop_left(StcStringView *sv, size_t n);

/**
 * Chops off the last n characters from the string view and returns a string
 * view to the chopped off characters whilst updating the given string view to
 * end before the first chopped off character.
 *
 * @param[in,out] sv the pointer to the string view to chop
 * @param[in] n the number of characters to chop from the right
 *
 * @return a string view to the characters that were chopped off
 */
StcStringView stc_sv_chop_right(StcStringView *sv, size_t n);

/**
 * Finds the index of the character c in the string view if it is contained in
 * the string view.
 *
 * @param[in] sv the string view to find the index of c in
 * @param[in] c the character to find the index of
 * @param[out] idx the pointer to save the index of c in only if found
 *
 * @return whether the character was found in the string view
 */
int stc_sv_index_of(StcStringView sv, char c, size_t *idx);

/**
 * Compares two string views.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_sv_cmp(StcStringView a, StcStringView b);

/**
 * Determines if two string views are equal.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a non-zero value if a and b are equal; else 0 if they are not equal
 */
int stc_sv_eq(StcStringView a, StcStringView b);

/**
 * Determines if two string views are equal in a case-insesitive manner.
 *
 * @param[in] a the left side string view in the comparison
 * @param[in] b the right side string view in the comparison
 *
 * @return a non-zero value if a and b are equal whilst ignoring case; else 0 if
 *         they are not equal whilst ignoring case
 */
int stc_sv_eq_ignorecase(StcStringView a, StcStringView b);

/**
 * Checks if a string view is a prefix of the other string view.
 *
 * @param[in] sv the string view to check the prefix of
 * @param[in] prefix the string view to test if it is a prefix of sv
 *
 * @return a non-zero value of the prefix string view is a prefix of sv; else 0
 *         if it is not
 */
int stc_sv_starts_with(StcStringView sv, StcStringView prefix);

/**
 * Checks if a string view is a suffix of the other string view.
 *
 * @param[in] sv the string view to check the suffix of
 * @param[in] suffix the string view to test if it is a suffix of sv
 *
 * @return a non-zero value of the suffix string view is a suffix of sv; else 0
 *         if it is not
 */
int stc_sv_ends_with(StcStringView sv, StcStringView suffix);

/**
 * Converts the string view into an unsigned integer, ignoring any junk after
 * the last contiguous digit. If the first character is not a digit then zero is
 * returned.
 *
 * @param[in] sv the string view to parse the unsigned integer from
 *
 * @return the unsigned integer parsed from the string view
 */
size_t stc_sv_to_int(StcStringView sv);

/**
 * Converts the string view into an unsigned integer, ignoring any junk after
 * the last contiguous digit. The string view is changed to view from the
 * character after the last contiguous digit. If the first character is not a
 * digit then zero is returned and the string view is not modified. If there are
 * no characters after the last contiguous digit, then the string view will have
 * length 0 and point to the character after the end of the string it used to
 * view.
 *
 * @param[in] sv the pointer to the string view to parse the unsigned integer
 *               from
 *
 * @return the unsigned integer parsed from the string view
 */
size_t stc_sv_chop_int(StcStringView *sv);

#endif /* STC_STRING_VIEW_H */
