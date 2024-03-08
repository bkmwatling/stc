#ifndef STC_UTF_H
#define STC_UTF_H

#include <stddef.h>

#if !defined(STC_DISABLE_FATP) && !defined(STC_DISABLE_SV) && \
    !defined(STC_UTF_DISABLE_SV)
#    include "../fatp/string_view.h"
#endif

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_UTF_ENABLE_SHORT_NAMES)
#    define UNICODE_ESCAPE_FMT STC_UNICODE_ESCAPE_FMT
#    define UNICODE_ESCAPE_ARG STC_UNICODE_ESCAPE_ARG

#    define unicode_from_escape_seq stc_unicode_from_escape_seq
#    define unicode_isprint         stc_unicode_isprint

#    define UTF8_IS_CONTINUATION STC_UTF8_IS_CONTINUATION
#    define UTF8_IS_SINGLE       STC_UTF8_IS_SINGLE
#    define UTF8_IS_DOUBLE       STC_UTF8_IS_DOUBLE
#    define UTF8_IS_TRIPLE       STC_UTF8_IS_TRIPLE
#    define UTF8_IS_QUADRUPLE    STC_UTF8_IS_QUADRUPLE

#    define utf8_is_valid       stc_utf8_is_valid
#    define utf8_isprint        stc_utf8_isprint
#    define utf8_nbytes         stc_utf8_nbytes
#    define utf8_clone          stc_utf8_clone
#    define utf8_cmp            stc_utf8_cmp
#    define utf8_try_cmp        stc_utf8_try_cmp
#    define utf8_to_codepoint   stc_utf8_to_codepoint
#    define utf8_from_codepoint stc_utf8_from_codepoint

#    define utf8_str_is_valid    stc_utf8_str_is_valid
#    define utf8_str_ncodepoints stc_utf8_str_ncodepoints
#    define utf8_str_next        stc_utf8_str_next
#    define utf8_str_advance     stc_utf8_str_advance

#    ifndef STC_UTF_DISABLE_SV
#        define utf8_is_valid_sv     stc_utf8_is_valid_sv
#        define utf8_nbytes_sv       stc_utf8_nbytes_sv
#        define utf8_to_sv           stc_utf8_to_sv
#        define utf8_cmp_sv          stc_utf8_cmp_sv
#        define utf8_try_cmp_sv      stc_utf8_try_cmp_sv
#        define utf8_to_codepoint_sv stc_utf8_to_codepoint_sv

#        define utf8_sv_is_valid    stc_utf8_sv_is_valid
#        define utf8_sv_ncodepoints stc_utf8_sv_ncodepoints
#        define utf8_sv_next        stc_utf8_sv_next
#        define utf8_sv_advance     stc_utf8_sv_advance
#    endif /* STC_UTF_DISABLE_SV */
#endif     /* STC_UTF_ENABLE_SHORT_NAMES */

/* --- Unicode codepoint macros and functions ------------------------------- */

#define STC_UNICODE_ESCAPE_FMT "\\%c%0*x"
#define STC_UNICODE_ESCAPE_ARG(codepoint)                                 \
    ((codepoint) <= 0xffff ? 'u' : 'U'), ((codepoint) <= 0xffff ? 4 : 8), \
        (codepoint)

/**
 * Decode a Unicode codepoint from the \uxxxx or \Uxxxxxxxx notation.
 *
 * @param[in] esc_seq the escaped Unicode sequence
 *
 * @return the decoded Unicode codepoint if the escape sequence is valid; else
 *         SIZE_MAX
 */
size_t stc_unicode_from_escape_seq(const char *esc_seq);

/**
 * Determine whether the Unicode codepoint represents a printable character.
 *
 * @param[in] codepoint the Unicode codepoint to check
 *
 * @return truthy value if the Unicode codepoint is printable; else 0
 */
int stc_unicode_isprint(size_t codepoint);

/* --- Single UTF-8 "character" macros and functions ------------------------ */

#define STC_UTF8_IS_CONTINUATION(c) (((c) & 0xc0) == 0x80)
#define STC_UTF8_IS_SINGLE(s)       (((s)[0] & 0x80) == 0x0)
#define STC_UTF8_IS_DOUBLE(s) \
    (((s)[0] & 0xe0) == 0xc0 && STC_UTF8_IS_CONTINUATION((s)[1]))
#define STC_UTF8_IS_TRIPLE(s)                                       \
    (((s)[0] & 0xf0) == 0xe0 && STC_UTF8_IS_CONTINUATION((s)[1]) && \
     STC_UTF8_IS_CONTINUATION((s)[2]))
#define STC_UTF8_IS_QUADRUPLE(s)                                    \
    (((s)[0] & 0xf8) == 0xf0 && STC_UTF8_IS_CONTINUATION((s)[1]) && \
     STC_UTF8_IS_CONTINUATION((s)[2]) && STC_UTF8_IS_CONTINUATION((s)[3]))

#define stc_utf8_is_valid(ch) (stc_utf8_nbytes((ch)) > 0)

#define stc_uft8_isprint(ch) stc_unicode_isprint(stc_utf8_to_codepoint(ch))

/**
 * Determines the number of bytes the UTF-8 "character" starting at the
 * character pointer (string) takes up.
 *
 * @param[in] ch the pointer to the first character in the UTF-8 "character"
 *
 * @return the number of bytes the UTF-8 "character" takes up if it is a valid
 *         encoded UTF-8 "character"; else zero
 */
unsigned int stc_utf8_nbytes(const char *ch);

/**
 * Creates a clone of the UTF-8 "character" into its own null-terminated string.
 * If the UTF-8 "character" is invalid, this function returns NULL.
 *
 * @param[in] ch the pointer to the first character in the UTF-8 "character"
 *
 * @return a pointer to the null-terminated string containing just the single
 *         UTF-8 "character" if it is valid; else NULL if it is invalid
 */
char *stc_utf8_clone(const char *ch);

/**
 * Compares two UTF-8 "characters". Assumes both UTF-8 "characters" are valid.
 *
 * @param[in] a the left side UTF-8 "character" in comparison
 * @param[in] b the right side UTF-8 "character" in comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_utf8_cmp(const char *a, const char *b);

/**
 * Tries to compare two UTF-8 "characters". If the two UTF-8 "characters" are
 * valid then this function stores the same result as calling stc_utf8_cmp in
 * the out parameter.
 *
 * Note: if cmp is NULL, it is ignored, i.e. if cmp NULL then this function is a
 * convenient way to check if two UTF-8 "characters" are valid at once.
 *
 * @param[in]  a   the left side UTF-8 "character" in comparison
 * @param[in]  b   the right side UTF-8 "character" in comparison
 * @param[out] cmp the value of the comparison (same as stc_utf8_cmp) if both
 *                 a and b are valid UTF-8 "characters"
 *
 * @return 0 if both a and b are valid UTF-8 "characters", else 1 if a is not a
 *         valid UTF-8 "character", else 2 if b is not a valid UTF-8
 *         "character", otherwise 3 if both a and b are not valid UTF-8
 *         "characters"
 */
int stc_utf8_try_cmp(const char *a, const char *b, int *cmp);

/**
 * Convert the UTF-8 "character" to the equivalent Unicode codepoint.
 *
 * @param[in] ch the UTF-8 "character" to convert
 *
 * @return the equivalent Unicode codepoint if the UTF-8 "character" is valid;
 *         else SIZE_MAX
 */
size_t stc_utf8_to_codepoint(const char *ch);

/**
 * Convert the Unicode codepoint to the equivalent UTF-8 "character".
 *
 * NOTE: the returned UTF-8 "character" is allocated with malloc (if not NULL)
 *       and thus needs to be freed once done with.
 *
 * @param[in] codepoint the Unicode codepoint to convert
 *
 * @return the equivalent NULL terminated UTF-8 "character" if the Unicode
 *         codepoint is valid (is less than 0x10ffff); else NULL
 */
char *stc_utf8_from_codepoint(size_t codepoint);

/* --- UTF-8 encoded strings macros and functions --------------------------- */

#define stc_utf8_str_is_valid(s) (stc_utf8_str_ncodepoints(s) > 0)

/**
 * Counts the number of Unicode codepoints in the string if it is a valid UTF-8
 * encoded string. If the string is not a valid UTF-8 encoded string, then this
 * function returns 0.
 *
 * @param[in] s the string to count the number of Unicode codepoints
 *
 * @return the number of Unicode codepoints in the string if it is a valid UTF-8
 *         encoded string; else 0 if it is invalid
 */
size_t stc_utf8_str_ncodepoints(const char *s);

/**
 * Retrieves the next UTF-8 "character" in the string if the current character
 * of the string is the start of a valid UTF-8 "character". If the character is
 * not the start of a valid UTF-8 "character" then NULL is returned.
 *
 * @param[in] s the string to advance to the next UTF-8 "character" from
 *
 * @return a pointer to the start of the next UTF-8 "character" in the string if
 *         the current character of the string is the start of a valid UTF-8
 *         "character"; else NULL
 */
const char *stc_utf8_str_next(const char *s);

/**
 * Advances the UTF-8 string pointed to by s to the next UTF-8 "character" and
 * returns the current UTF-8 "character" of the string if the current character
 * of the string is the start of a valid UTF-8 "character". If the character is
 * not the start of a valid UTF-8 codepoint then NULL is returned and the string
 * is left unchanged.
 *
 * @param[in] s the string to advance to the next UTF-8 "character"
 *
 * @return a pointer to the start of the current UTF-8 "character" in the string
 *         if the current character of the string is the start of a valid UTF-8
 *         "character"; else NULL
 */
const char *stc_utf8_str_advance(const char **s);

/* --- UTF-8 string view macros and functions ------------------------------- */

#if !defined(STC_DISABLE_FATP) && !defined(STC_DISABLE_SV) && \
    !defined(STC_UTF_DISABLE_SV)

/* --- Single UTF-8 "character" macros and functions for string views ------- */

#    define stc_utf8_is_valid_sv(sv) (stc_utf8_nbytes((sv)) > 0)

/**
 * Determines the number of bytes the UTF-8 "character" starting at the
 * string view with certain length. If the UTF-8 "character" is invalid, this
 * function returns NULL. The length of the string view is checked and so if the
 * underlying bytes indicate the UTF-8 "character" exceeds the string view
 * length, it is deemed invalid.
 *
 * @param[in] sv the string view starting at the first character in the UTF-8
 *               "character"
 *
 * @return the number of bytes the UTF-8 "character" takes up if it is a valid
 *         encoded UTF-8 "character" in the context of a string view (codepoint
 *         does not go past string view bounds); else zero
 */
unsigned int stc_utf8_nbytes_sv(StcStringView sv);

/**
 * Creates a string view of the UTF-8 "character" with the correct length.
 * An advantage of using this function over stc_utf8_clone is that is does not
 * need to perform memory allocation.
 *
 * @param[in] ch the pointer to the first character in the UTF-8 "character"
 *
 * @return a string view of the UTF-8 "character"
 */
StcStringView stc_utf8_to_sv(const char *ch);

/**
 * Compares two UTF-8 "characters" in the context of string views. Assumes both
 * UTF-8 codepoints are valid.
 *
 * @param[in] a the left side UTF-8 "character" string view in comparison
 * @param[in] b the right side UTF-8 "character" string view in comparison
 *
 * @return a value less than 0 if a < b, else a value greater than 0 if a > b,
 *         otherwise 0 meaning a == b
 */
int stc_utf8_cmp_sv(StcStringView a, StcStringView b);

/**
 * Tries to compare two UTF-8 "characters" in the context of string views. If
 * the two UTF-8 "characters" are valid then this function stores the same
 * result as calling stc_utf8_cmp_sv in the out parameter.
 *
 * Note: if cmp is NULL, it is ignored, i.e. if cmp NULL then this function is a
 * convenient way to check if two UTF-8 "character" string views are valid at
 * once.
 *
 * @param[in]  a   the left side UTF-8 "character" string view in comparison
 * @param[in]  b   the right side UTF-8 "character" string view in comparison
 * @param[out] cmp the value of the comparison (same as stc_utf8_cmp_sv) if both
 *                 a and b are valid UTF-8 "character" string views
 *
 * @return 0 if both a and b are valid UTF-8 "character" string views, else 1 if
 *         a is not a valid UTF-8 "character" string view, else 2 if b is not a
 *         valid UTF-8 "character" string view, otherwise 3 if both a and b are
 *         not valid UTF-8 "character" string views
 */
int stc_utf8_try_cmp_sv(StcStringView a, StcStringView b, int *cmp);

/**
 * Convert the UTF-8 "character" string view to the equivalent Unicode
 * codepoint.
 *
 * @param[in] sv the UTF-8 "character" string view to convert
 *
 * @return the equivalent Unicode codepoint if the UTF-8 "character" string view
 *         is valid; else SIZE_MAX
 */
size_t stc_utf8_to_codepoint_sv(StcStringView sv);

/* --- UTF-8 encoded strings macros and functions for string views ---------- */

#    define stc_utf8_sv_is_valid(s) (stc_utf8_sv_ncodepoints(s) > 0)

/**
 * Counts the number of Unicode codepoints in the string view if it is a valid
 * UTF-8 encoded string view. If the string view is not of a valid UTF-8 encoded
 * string, then this function returns 0.
 *
 * @param[in] sv the string view to count the number of Unicode codepoints
 *
 * @return the number of Unicode codepoints in the string view if it is of a
 *         valid UTF-8 encoded string; else 0 if it is invalid
 */
size_t stc_utf8_sv_ncodepoints(StcStringView sv);

/**
 * Retrieves the next UTF-8 "character" in the string view if the starting
 * character of the string view is the start of a valid UTF-8 "character". If
 * the character is not the start of a valid UTF-8 "character" then a string
 * view of length 0 is returned.
 *
 * @param[in] sv the string view to advance to the next UTF-8 "character" from
 *
 * @return a string view to the start of the next UTF-8 "character" in the
 *         string view if the current character of the string view is the start
 *         of a valid UTF-8 "character"; else a string view of length 0
 */
StcStringView stc_utf8_sv_next(StcStringView sv);

/**
 * Advances the UTF-8 string view pointed to by sv to the next UTF-8 "character"
 * and returns the current UTF-8 "character" of the string view as a string view
 * if the current character of the string view is the start of a valid UTF-8
 * "character". If the character is not the start of a valid UTF-8 "character"
 * then a string view of length 0 is returned and the given string view is
 * left unchanged.
 *
 * @param[in] sv the string view pointer to advance to next UTF-8 "character"
 *
 * @return a string view to the start of the current UTF-8 "character" in the
 *         string view if the current character of the string view is the start
 *         of a valid UTF-8 "character"; else a string view of length 0
 */
StcStringView stc_utf8_sv_advance(StcStringView *sv);

#endif /* STC_UTF_DISABLE_SV */

#endif /* STC_UTF_H */
