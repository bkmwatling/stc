#ifndef STC_STRING_H
#define STC_STRING_H

#include <stdarg.h>

#include "slice.h"
#include "vec.h"

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_STRING_ENABLE_SHORT_NAMES)
typedef StcString String;

#    define STRING_DEFAULT_CAP STC_STRING_DEFAULT_CAP
#    define string_header      stc_string_header

#    define string_new     stc_string_new
#    define string_default stc_string_default
#    define string_clone   stc_string_clone
#    define string_free    stc_string_free

#    define string_len        stc_string_len
#    define string_cap        stc_string_cap
#    define string_len_unsafe stc_string_len_unsafe
#    define string_cap_unsafe stc_string_cap_unsafe
#    define string_is_empty   stc_string_is_empty
#    define string_clear      stc_string_clear

#    define string_push_back  stc_string_push_back
#    define string_push_front stc_string_push_front
#    define string_pop        stc_string_pop
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

/** Simple type definition to show intention of using resizable string type. */
typedef StcVec(char) StcString;

#define STC_STRING_DEFAULT_CAP 32
#define stc_string_header      stc_vec_header

#define stc_string_new(cap)  stc_vec_new(sizeof(char), (cap))
#define stc_string_default() stc_string_new(STC_STRING_DEFAULT_CAP)
#define stc_string_clone     stc_vec_clone
#define stc_string_free      stc_vec_free

#define stc_string_len        stc_vec_len
#define stc_string_cap        stc_vec_cap
#define stc_string_len_unsafe stc_vec_len_unsafe
#define stc_string_cap_unsafe stc_vec_cap_unsafe
#define stc_string_is_empty   stc_vec_is_empty
#define stc_string_clear      stc_vec_clear

#define stc_string_push_back  stc_vec_push_back
#define stc_string_push_front stc_vec_push_front
#define stc_string_pop        stc_vec_pop
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
#define stc_string_push_vfmt(s, fmt, ap) \
    _stc_string_push_vfmt(&(s), (fmt), (ap))
#define stc_string_push_fmt(s, fmt, ...) \
    _stc_string_push_fmt(&(s), (fmt), __VA_ARGS__)

/* --- Resizable string functions ------------------------------------------- */

/**
 * Pushes a string onto the resizable string created from the format specifier
 * and variable argument list.
 *
 * @param[in] self the pointer to the resizable string
 * @param[in] fmt  the format specifier
 * @param[in] ap   the variable argument list to parse with the format specifier
 */
void _stc_string_push_vfmt(char **self, const char *fmt, va_list ap);

/**
 * Pushes a string onto the resizable string created from the format specifier
 * and variable arguments.
 *
 * @param[in] self the pointer to the resizable string
 * @param[in] fmt  the format specifier
 * @param[in] ...  the variable arguments to parse with the format specifier
 */
void _stc_string_push_fmt(char **self, const char *fmt, ...);

/* --- String slice definitions (aliases to slice) -------------------------- */

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_STR_ENABLE_SHORT_NAMES)
typedef StcStr Str;

#    define str_header     stc_str_header
#    define str_new        stc_str_new
#    define str_from_parts stc_str_from_parts
#    define str_from_cstr  stc_str_from_cstr
#    define str_from_lit   stc_str_from_lit
#    define str_clone      stc_str_clone
#    define str_free       stc_str_free

#    define str_len        stc_str_len
#    define str_len_unsafe stc_str_len_unsafe
#    define str_first      stc_str_first
#    define str_last       stc_str_last

#    define str_from_vfmt stc_str_from_vfmt
#    define str_from_fmt  stc_str_from_fmt
#endif /* STC_STR_ENABLE_SHORT_NAMES */

/** Simple type definition to show intention of using string slice type. */
typedef StcSlice(char) StcStr;

#define stc_str_header          stc_slice_header
#define stc_str_new(len)        stc_slice_new(sizeof(char), (len))
#define stc_str_from_parts      stc_slice_from_parts
#define stc_str_from_cstr(cstr) stc_str_from_parts(cstr, strlen((cstr)))
#define stc_str_from_lit(s)     stc_str_from_parts(s, sizeof(s) - 1)
#define stc_str_clone           stc_slice_clone
#define stc_str_free            stc_slice_free

#define stc_str_len        stc_slice_len
#define stc_str_len_unsafe stc_slice_len_unsafe
#define stc_str_first      stc_slice_first
#define stc_str_last       stc_slice_last

/* --- String slice functions ----------------------------------------------- */

/**
 * Creates a string slice from the format specifier and variable argument list.
 *
 * @param[in] fmt the format specifier
 * @param[in] ap  the variable argument list to parse with the format specifier
 *
 * @return a string slice constructed from the format specifier and variable
 *         argument list
 */
char *stc_str_from_vfmt(const char *fmt, va_list ap);

/**
 * Creates a string slice from the format specifier and variable arguments.
 *
 * @param[in] fmt the format specifier
 * @param[in] ... the variable arguments to parse with the format specifier
 *
 * @return a string slice constructed from the format specifier and variable
 *         arguments
 */
char *stc_str_from_fmt(const char *fmt, ...);

#endif /* STC_STRING_H */
