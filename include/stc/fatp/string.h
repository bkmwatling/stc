#ifndef STC_STRING_H
#define STC_STRING_H

#include <stdarg.h>
#include <string.h>

#include <stc/fatp/slice.h>
#include <stc/fatp/vec.h>

/** Simple type definition to show intention of using resizable string type. */
typedef StcVec(char) StcString;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_STRING_ENABLE_SHORT_NAMES)
typedef StcString String;

#    define STRING_DEFAULT_CAP STC_STRING_DEFAULT_CAP

#    define string_new     stc_string_new
#    define string_default stc_string_default
#    define string_clone   stc_string_clone
#    define string_free    stc_string_free

#    define string_is_empty stc_string_is_empty
#    define string_clear    stc_string_clear

#    define string_push_back  stc_string_push_back
#    define string_push_front stc_string_push_front
#    define string_pop_back   stc_string_pop_back
#    define string_pop_front  stc_string_pop_front
#    define string_at         stc_string_at
#    define string_first      stc_string_first
#    define string_last       stc_string_last

#    define string_insert   stc_string_insert
#    define string_remove   stc_string_remove
#    define string_drain    stc_string_drain
#    define string_truncate stc_string_truncate

#    define string_append          stc_string_append
#    define string_extend          stc_string_extend
#    define string_extend_from_str stc_string_extend_from_str

#    define string_reserve       stc_string_reserve
#    define string_reserve_exact stc_string_reserve_exact
#    define string_reserve_index stc_string_reserve_index
#    define string_shrink        stc_string_shrink
#    define string_shrink_to_fit stc_string_shrink_to_fit

#    define string_as_str stc_string_as_str
#    define string_to_str stc_string_to_str

#    define string_push_cstr stc_string_push_cstr
#    define string_push_vfmt stc_string_push_vfmt
#    define string_push_fmt  stc_string_push_fmt
#endif /* STC_STRING_ENABLE_SHORT_NAMES */

/* --- Resizable string definitions (aliases to vec) ------------------------ */

#define STC_STRING_DEFAULT_CAP 32

#define stc_string_new(cap)  stc_vec_new(char, cap)
#define stc_string_default() stc_string_new(STC_STRING_DEFAULT_CAP)
#define stc_string_clone     stc_vec_clone
#define stc_string_free      stc_vec_free

#define stc_string_is_empty stc_vec_is_empty
#define stc_string_clear    stc_vec_clear

#define stc_string_push_back  stc_vec_push_back
#define stc_string_push_front stc_vec_push_front
#define stc_string_pop_back   stc_vec_pop_back
#define stc_string_pop_front  stc_vec_pop_front
#define stc_string_at         stc_vec_at
#define stc_string_first      stc_vec_first
#define stc_string_last       stc_vec_last

#define stc_string_insert   stc_vec_insert
#define stc_string_remove   stc_vec_remove
#define stc_string_drain    stc_vec_drain
#define stc_string_truncate stc_vec_truncate

#define stc_string_append          stc_vec_append
#define stc_string_extend          stc_vec_extend
#define stc_string_extend_from_str stc_vec_extend_from_slice

#define stc_string_reserve       stc_vec_reserve
#define stc_string_reserve_exact stc_vec_reserve_exact
#define stc_string_reserve_index stc_vec_reserve_index
#define stc_string_shrink        stc_vec_shrink
#define stc_string_shrink_to_fit stc_vec_shrink_to_fit

#define stc_string_as_str stc_vec_as_slice
#define stc_string_to_str stc_vec_to_slice

#define stc_string_push_cstr(s, cstr) stc_string_push_fmt(s, cstr)

/* --- Resizable string functions ------------------------------------------- */

/**
 * Push a string onto the resizable string created from the format specifier and
 * variable argument list.
 *
 * @param[in] self the pointer to the resizable string
 * @param[in] fmt  the format specifier
 * @param[in] ap   the variable argument list to parse with the format specifier
 */
void stc_string_push_vfmt(StcString *self, const char *fmt, va_list ap);

/**
 * Push a string onto the resizable string created from the format specifier and
 * variable arguments.
 *
 * @param[in] self the pointer to the resizable string
 * @param[in] fmt  the format specifier
 * @param[in] ...  the variable arguments to parse with the format specifier
 */
void stc_string_push_fmt(StcString *self, const char *fmt, ...);

/* --- String slice definitions (aliases to slice) -------------------------- */

/** Simple type definition to show intention of using string slice type. */
typedef StcSlice(char) StcStr;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_STR_ENABLE_SHORT_NAMES)
typedef StcStr Str;

#    define STR_FMT STC_STR_FMT
#    define STR_ARG STC_STR_ARG

#    define str_new        stc_str_new
#    define str_from_parts stc_str_from_parts
#    define str_from_cstr  stc_str_from_cstr
#    define str            stc_str
#    define str_clone      stc_str_clone
#    define str_free       stc_str_free

#    define str_at    stc_str_at
#    define str_first stc_str_first
#    define str_last  stc_str_last

#    define str_substr       stc_str_substr
#    define str_substr_from  stc_str_substr_from
#    define str_substr_until stc_str_substr_until

#    define str_from_vfmt stc_str_from_vfmt
#    define str_from_fmt  stc_str_from_fmt
#endif /* STC_STR_ENABLE_SHORT_NAMES */

/**
 * printf macros for StcStr
 * USAGE:
 *   StcStr name = ...;
 *   printf("Name: " STC_STR_FMT "\n", STC_STR_ARG(name));
 */
#define STC_STR_FMT      "%.*s"
#define STC_STR_ARG(str) (int) (str).len, (str).__stc_slice_data

/**
 * Create a new string slice with given length, allocating the necessary
 * underlying memory.
 *
 * @param[in] len the length of the string slice to create
 *
 * @return the created string slice
 */
#define stc_str_new(len) stc_slice_new(char, len)

/**
 * Create a string slice from a character pointer and length by copying the
 * string pointed to by the pointer.
 *
 * @param[in] s   the string address to copy from
 * @param[in] len the length of the string slice
 *
 * @return a string slice of the given string over length of len
 */
#define stc_str_from_parts(s, len) stc_slice_from_parts(char, s, len)

/**
 * Create a string slice of the range between two strings (character pointers)
 * by copying that range of characters.
 *
 * @param[in] start the start (pointer) of the string range
 * @param[in] end   the end (pointer) of the string range
 *
 * @return a string slice of the copied string defined by the range
 */
#define stc_str_from_range(start, end) \
    stc_str_from_parts(start, (end) - (start))

/**
 * Create a string slice from a C (null terminated) string by calling strlen for
 * automatically by copying the C string.
 *
 * @param[in] cstr the C string to copy
 *
 * @return a string slice of the copied C string
 */
#define stc_str_from_cstr(cstr) stc_str_from_parts(cstr, strlen((cstr)))

/**
 * Create a string slice from a string literal by copying the literal.
 *
 * @param[in] lit the string literal to copy
 *
 * @return a string slice of the copied string literal
 */
#define stc_str(lit) stc_str_from_parts(lit, sizeof(lit) - 1)

/**
 * Create a clone of a string slice by copying the underlying character array.
 *
 * @param[in] str the string slice to clone
 *
 * @return the clone of the string slice
 */
#define stc_str_clone stc_slice_clone

/**
 * Free the underlying memory of a string slice.
 *
 * @param[in] str the string slice to free the underlying memory of
 */
#define stc_str_free stc_slice_free

/**
 * Get the character at the specified index of a string slice.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 *
 * @param[in] str the string slice to get the indexed character from
 * @param[in] i   the index of the character to retrieve
 *
 * @return the indexed character from the string slice
 */
#define stc_str_at stc_slice_at

/**
 * Get the first element from a string slice.
 *
 * @param[in] str the string slice to retrieve the first element of
 *
 * @return the first element of the string slice
 */
#define stc_str_first stc_slice_first

/**
 * Get the last element from a string slice.
 *
 * @param[in] str the string slice to retrieve the last element of
 *
 * @return the last element of the string slice
 */
#define stc_str_last stc_slice_last

/**
 * Create a substring slice from a string slice from the starting index until
 * the ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The substring slice is a copy of the string slice over the defined
 *       range, and thus needs to be freed.
 *
 * @param[in] str   the string slice to create the substring slice from
 * @param[in] start the start index of the substring slice
 * @param[in] end   the end index of the substring slice
 *
 * @return a substring slice over the defined range
 */
#define stc_str_substr stc_slice_subslice

/**
 * Create a substring slice from a string slice from the starting index until
 * the end of the string slice.
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The substring slice is a copy of the string slice over the defined
 *       range, and thus needs to be freed.
 *
 * @param[in] str   the string slice to create the substring slice from
 * @param[in] start the start index of the substring slice
 *
 * @return a substring slice from the starting index until the end of the string
 *         slice
 */
#define stc_str_substr_from(str, start) stc_str_substr(str, start, (str).len)

/**
 * Create a substring slice from a string slice from the start of the string
 * slice until the ending index (non-inclusive).
 *
 * NOTE: No index bounds checks are performed for efficiency.
 * NOTE: The substring slice is a copy of the string slice over the defined
 *       range, and thus needs to be freed.
 *
 * @param[in] str the string slice to create the substring slice from
 * @param[in] end the end index of the substring slice
 *
 * @return a substring slice from the start of the string slice until the ending
 *         index (non-inclusive)
 */
#define stc_str_substr_until(str, end) stc_str_substr(str, 0, end)

/* --- String slice functions ----------------------------------------------- */

/**
 * Create a string slice from the format specifier and variable argument list.
 *
 * @param[in] fmt the format specifier
 * @param[in] ap  the variable argument list to parse with the format specifier
 *
 * @return a string slice constructed from the format specifier and variable
 *         argument list
 */
StcStr stc_str_from_vfmt(const char *fmt, va_list ap);

/**
 * Create a string slice from the format specifier and variable arguments.
 *
 * @param[in] fmt the format specifier
 * @param[in] ... the variable arguments to parse with the format specifier
 *
 * @return a string slice constructed from the format specifier and variable
 *         arguments
 */
StcStr stc_str_from_fmt(const char *fmt, ...);

#endif /* STC_STRING_H */
