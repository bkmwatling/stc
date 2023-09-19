#ifndef STC_UTF_H
#define STC_UTF_H

#include <stddef.h>

#ifndef STC_UTF_DISABLE_SV
#    include "../fatp/string_view.h"
#endif /* STC_UTF_DISABLE_SV */

#ifdef STC_UTF_ENABLE_SHORT_NAMES
#    define UTF8_IS_CONTINUATION STC_UTF8_IS_CONTINUATION
#    define UTF8_IS_SINGLE       STC_UTF8_IS_SINGLE
#    define UTF8_IS_DOUBLE       STC_UTF8_IS_DOUBLE
#    define UTF8_IS_TRIPLE       STC_UTF8_IS_TRIPLE
#    define UTF8_IS_QUADRUPLE    STC_UTF8_IS_QUADRUPLE

#    define utf8_is_valid stc_utf8_is_valid
#    define utf8_nbytes   stc_utf8_nbytes
#    define utf8_to_str   stc_utf8_to_str
#    define utf8_cmp      stc_utf8_cmp
#    define utf8_try_cmp  stc_utf8_try_cmp

#    define utf8_str_is_valid    stc_utf8_str_is_valid
#    define utf8_str_ncodepoints stc_utf8_str_ncodepoints
#    define utf8_str_next        stc_utf8_str_next

#    ifndef STC_UTF_DISABLE_SV
#        define utf8_is_valid_sv stc_utf8_is_valid_sv
#        define utf8_nbytes_sv   stc_utf8_nbytes_sv
#        define utf8_to_sv       stc_utf8_to_sv
#        define utf8_cmp_sv      stc_utf8_cmp_sv
#        define utf8_try_cmp_sv  stc_utf8_try_cmp_sv

#        define utf8_sv_is_valid    stc_utf8_sv_is_valid
#        define utf8_sv_ncodepoints stc_utf8_sv_ncodepoints
#        define utf8_sv_next        stc_utf8_sv_next
#    endif /* STC_UTF_DISABLE_SV */
#endif     /* STC_UTF_ENABLE_SHORT_NAMES */

/*** Single UTF-8 codepoint macros and functions ******************************/

#define STC_UTF8_IS_CONTINUATION(c) (((c) &0xc0) == 0x80)
#define STC_UTF8_IS_SINGLE(s)       (((s)[0] & 0x80) == 0x0)
#define STC_UTF8_IS_DOUBLE(s) \
    (((s)[0] & 0xe0) == 0xc0 && STC_UTF8_IS_CONTINUATION((s)[1]))
#define STC_UTF8_IS_TRIPLE(s)                                       \
    (((s)[0] & 0xf0) == 0xe0 && STC_UTF8_IS_CONTINUATION((s)[1]) && \
     STC_UTF8_IS_CONTINUATION((s)[2]))
#define STC_UTF8_IS_QUADRUPLE(s)                                    \
    (((s)[0] & 0xf8) == 0xf0 && STC_UTF8_IS_CONTINUATION((s)[1]) && \
     STC_UTF8_IS_CONTINUATION((s)[2]) && STC_UTF8_IS_CONTINUATION((s)[3]))

#define stc_utf8_is_valid(codepoint) (stc_utf8_nbytes((codepoint)) > 0)

/**
 * Determines the number of bytes the UTF-8 codepoint starting at the character
 * pointer (string) takes up.
 *
 * @param[in] codepoint the pointer to the first character in the codepoint
 *
 * @return the number of bytes the codepoint takes up if it is a valid
 *         codepoint; else zero
 */
int stc_utf8_nbytes(const char *codepoint);

/**
 * Creates a copy of the UTF-8 codepoint into its own null-terminated string.
 * If the UTF-8 codepoint is invalid, this function returns NULL.
 *
 * @param[in] codepoint the pointer to the first character in the codepoint
 *
 * @return a pointer to the null-terminated string containing just the single
 *         UTF-8 codepoint if it is valid; else NULL if it is invalid
 */
char *stc_utf8_to_str(const char *codepoint);

/**
 * Compares two UTF-8 codepoints. Assumes both UTF-8 codepoints are valid.
 *
 * @param[in] a the left side UTF-8 codepoint in the comparison
 * @param[in] b the right side UTF-8 codepoint in the comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_utf8_cmp(const char *a, const char *b);

/**
 * Tries to compare two UTF-8 codepoints. If the two UTF-8 codepoints are valid
 * then this function returns the same result as calling stc_utf8_cmp.
 *
 * Note: if cmp is NULL, it is ignored, i.e. if cmp NULL then this function is a
 * convenient way to check if two UTF-8 codepoints are valid at once.
 *
 * @param[in] a the left side UTF-8 codepoint in the comparison
 * @param[in] b the right side UTF-8 codepoint in the comparison
 * @param[out] cmp the value of the comparison (same as stc_utf8_cmp) if both
 *                 a and b are valid UTF-8 codepoints
 *
 * @return 0 if both a and b are valid UTF-8 codepoints, else 1 if a is not a
 *         valid UTF-8 codepoint, else 2 if b is not a valid UTF-8 codepoint,
 *         otherwise 3 if both a and b are not valid UTF-8 codepoints
 */
int stc_utf8_try_cmp(const char *a, const char *b, int *cmp);

/*** UTF-8 encoded strings macros and functions *******************************/

#define stc_utf8_str_is_valid(s) (stc_utf8_str_ncodepoints(s) > 0)

/**
 * Counts the number of UTF-8 codepoints in the string if it is a valid UTF-8
 * encoded string. If the string is not a valid UTF-8 encoded string, then this
 * function returns 0.
 *
 * @param[in] s the string to count the number of UTF-8 codepoints
 *
 * @return the number of UTF-8 codepoints in the string if it is a valid UTF-8
 *         encoded string; else 0 if it is invalid
 */
size_t stc_utf8_str_ncodepoints(const char *s);

/**
 * Retrieves the next UTF-8 codepoint in the string if the current character of
 * the string is the start of a valid UTF-8 codepoint. If the character is not
 * the start of a valid UTF-8 codepoint then NULL is returned.
 *
 * @param[in] s the string to advance to the next UTF-8 codepoint from
 *
 * @return a pointer to the start of the next UTF-8 codepoint in the string if
 *         the current character of the string is the start of a valid UTF-8
 *         codepoint; else NULL
 */
const char *stc_utf8_str_next(const char *s);

/*** UTF-8 string view macros and functions ***********************************/

#ifndef STC_UTF_DISABLE_SV

/*** Single UTF-8 codepoint macros and functions for string views *************/

#    define stc_utf8_is_valid_sv(codepoint) (stc_utf8_nbytes((codepoint)) > 0)

/**
 * Determines the number of bytes the UTF-8 codepoint starting at the character
 * pointer (string) takes up in the context of a string view.
 *
 * @param[in] codepoint the string view starting at the first character in the
 *                      codepoint
 *
 * @return the number of bytes the codepoint takes up if it is a valid
 *         codepoint in the context of a string view (codepoint does not go past
 *         string view bounds); else zero
 */
int stc_utf8_nbytes_sv(StcStringView codepoint);

/**
 * Creates a string view of the UTF-8 codepoint with the correct length.
 * An advantage of using this function over stc_utf8_to_str is that is does not
 * need to perform memory allocation.
 *
 * @param[in] codepoint the pointer to the first character in the codepoint
 *
 * @return a string view of the UTF-8 codepoint
 */
StcStringView stc_utf8_to_sv(const char *codepoint);

/**
 * Compares two UTF-8 codepoints in the context of string views. Assumes both
 * UTF-8 codepoints are valid.
 *
 * @param[in] a the left side UTF-8 codepoint string view in the comparison
 * @param[in] b the right side UTF-8 codepoint string view in the comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_utf8_cmp_sv(StcStringView a, StcStringView b);

/**
 * Tries to compare two UTF-8 codepoints in the context of string views. If the
 * two UTF-8 codepoints are valid then this function returns the same result as
 * calling stc_utf8_cmp_sv.
 *
 * Note: if cmp is NULL, it is ignored, i.e. if cmp NULL then this function is a
 * convenient way to check if two UTF-8 codepoint string views are valid at
 * once.
 *
 * @param[in] a the left side UTF-8 codepoint string view in the comparison
 * @param[in] b the right side UTF-8 codepoint string view in the comparison
 * @param[out] cmp the value of the comparison (same as stc_utf8_cmp_sv) if both
 *                 a and b are valid UTF-8 codepoint string views
 *
 * @return 0 if both a and b are valid UTF-8 codepoint string views, else 1 if a
 *         is not a valid UTF-8 codepoint string view, else 2 if b is not a
 *         valid UTF-8 codepoint string view, otherwise 3 if both a and b are
 *         not valid UTF-8 codepoint string views
 */
int stc_utf8_try_cmp_sv(StcStringView a, StcStringView b, int *cmp);

/*** UTF-8 encoded strings macros and functions for string views **************/

#    define stc_utf8_sv_is_valid(s) (stc_utf8_sv_ncodepoints(s) > 0)

/**
 * Counts the number of UTF-8 codepoints in the string view if it is a valid
 * UTF-8 encoded string view. If the string view is not of a valid UTF-8 encoded
 * string, then this function returns 0.
 *
 * @param[in] sv the string view to count the number of UTF-8 codepoints
 *
 * @return the number of UTF-8 codepoints in the string view if it is of a valid
 *         UTF-8 encoded string; else 0 if it is invalid
 */
size_t stc_utf8_sv_ncodepoints(StcStringView sv);

/**
 * Retrieves the next UTF-8 codepoint in the string view if the starting
 * character of the string view is the start of a valid UTF-8 codepoint. If the
 * character is not the start of a valid UTF-8 codepoint then a string view of
 * length 0 is returned.
 *
 * @param[in] sv the string view to advance to the next UTF-8 codepoint from
 *
 * @return a string view to the start of the next UTF-8 codepoint in the string
 *         if the current character of the string view is the start of a valid
 *         UTF-8 codepoint; else a string view of length 0
 */
StcStringView stc_utf8_sv_next(StcStringView sv);

#endif /* STC_UTF_DISABLE_SV */

#endif /* STC_UTF_H */
