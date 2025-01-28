#ifndef STC_FATP_UTIL_H
#define STC_FATP_UTIL_H

#include <stc/util/macro.h>

/* --- Magic macros to handle multi-word types ------------------------------ */

// our dictionary
#define _STC_FATP_WORD_ptr      ptr,
#define _STC_FATP_WORD_void     void,
#define _STC_FATP_WORD_unsigned unsigned,
#define _STC_FATP_WORD_signed   signed,
#define _STC_FATP_WORD_char     char,
#define _STC_FATP_WORD_short    short,
#define _STC_FATP_WORD_int      int,
#define _STC_FATP_WORD_long     long,
#define _STC_FATP_WORD_size_t   size_t,
#define _STC_FATP_WORD_float    float,
#define _STC_FATP_WORD_double   double,
#define _STC_FATP_WORD_const    const,
#define _STC_FATP_WORD_enum     enum,
#define _STC_FATP_WORD_struct   struct,
#define _STC_FATP_WORD_union    union,

#define _STC_FATP_TYPE_ptr      *,
#define _STC_FATP_TYPE_void     void,
#define _STC_FATP_TYPE_unsigned unsigned,
#define _STC_FATP_TYPE_signed   signed,
#define _STC_FATP_TYPE_char     char,
#define _STC_FATP_TYPE_short    short,
#define _STC_FATP_TYPE_int      int,
#define _STC_FATP_TYPE_long     long,
#define _STC_FATP_TYPE_size_t   size_t,
#define _STC_FATP_TYPE_float    float,
#define _STC_FATP_TYPE_double   double,
#define _STC_FATP_TYPE_const    const,
#define _STC_FATP_TYPE_enum     enum,
#define _STC_FATP_TYPE_struct   struct,
#define _STC_FATP_TYPE_union    union,

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    define _STC_FATP_WORD_int8_t    int8_t,
#    define _STC_FATP_WORD_int16_t   int16_t,
#    define _STC_FATP_WORD_int32_t   int32_t,
#    define _STC_FATP_WORD_int64_t   int64_t,
#    define _STC_FATP_WORD_intmax_t  intmax_t,
#    define _STC_FATP_WORD_uint8_t   uint8_t,
#    define _STC_FATP_WORD_uint16_t  uint16_t,
#    define _STC_FATP_WORD_uint32_t  uint32_t,
#    define _STC_FATP_WORD_uint64_t  uint64_t,
#    define _STC_FATP_WORD_uintmax_t uintmax_t,

#    define _STC_FATP_TYPE_int8_t    int8_t,
#    define _STC_FATP_TYPE_int16_t   int16_t,
#    define _STC_FATP_TYPE_int32_t   int32_t,
#    define _STC_FATP_TYPE_int64_t   int64_t,
#    define _STC_FATP_TYPE_intmax_t  intmax_t,
#    define _STC_FATP_TYPE_uint8_t   uint8_t,
#    define _STC_FATP_TYPE_uint16_t  uint16_t,
#    define _STC_FATP_TYPE_uint32_t  uint32_t,
#    define _STC_FATP_TYPE_uint64_t  uint64_t,
#    define _STC_FATP_TYPE_uintmax_t uintmax_t,
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

// Join arguments with `_`.
#define __STC_FATP_JOIN_U(a, b) a##_##b
#define _STC_FATP_JOIN_U(a, b)  __STC_FATP_JOIN_U(a, b)
#define __STC_FATP_JOIN_TWO(a, b) \
    _STC_IFEMPTY(_STC_FIRST, _STC_FATP_JOIN_U, b)(a, b)
#define _STC_FATP_JOIN_TWO(a, b) __STC_FATP_JOIN_TWO(a, b)
#define _STC_FATP_JOIN(...)      _STC_FOREACH(_STC_FATP_JOIN_TWO, __VA_ARGS__)

// Join arguments with ` `.
#define __STC_FATP_JOIN_SPACE_U(a, b) a b
#define _STC_FATP_JOIN_SPACE_U(a, b)  __STC_FATP_JOIN_SPACE_U(a, b)
#define __STC_FATP_JOIN_SPACE_TWO(a, b) \
    _STC_IFEMPTY(_STC_FIRST, _STC_FATP_JOIN_SPACE_U, b)(a, b)
#define _STC_FATP_JOIN_SPACE_TWO(a, b) __STC_FATP_JOIN_SPACE_TWO(a, b)
#define _STC_FATP_JOIN_SPACE(...) \
    _STC_FOREACH(_STC_FATP_JOIN_SPACE_TWO, __VA_ARGS__)

// Append _STC_FATP_WORD_ to each argument and join arguments with spaces.
#define _STC_FATP_WORD_           /* the last one expands to empty */
#define _STC_FATP_WORDS_TWO(a, b) _STC_FATP_WORD_##a b
#define _STC_FATP_WORDS(...)      _STC_FOREACH(_STC_FATP_WORDS_TWO, __VA_ARGS__)

// Append _STC_FATP_TYPE_ to each argument and join arguments with spaces.
#define _STC_FATP_TYPE_           /* the last one expands to empty */
#define _STC_FATP_TYPES_TWO(a, b) _STC_FATP_TYPE_##a b
#define _STC_FATP_TYPES(...)      _STC_FOREACH(_STC_FATP_TYPES_TWO, __VA_ARGS__)

#define _STC_FATP_MAGIC_WORD_MACRO(a)                                    \
    _STC_FATP_JOIN(_STC_FATP_WORDS(                                      \
        _STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS( \
            _STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS(a)))))))))

#define _STC_FATP_MAGIC_TYPE_MACRO(a)                                    \
    _STC_FATP_JOIN_SPACE(_STC_FATP_TYPES(_STC_FATP_WORDS(                \
        _STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS( \
            _STC_FATP_WORDS(_STC_FATP_WORDS(_STC_FATP_WORDS(a))))))))))

/* --- Macros for auto-casting results of fat-pointers ---------------------- */

#define _STC_FATP_ID_TRANSFORM(x) x

#define _STC_FATP_AUTO_CAST_TYPE(T, from_transform, to_transform, expr, pntr, \
                                 unop)                                        \
    from_transform(T) : ((to_transform(T) pntr) unop(expr))

#define _STC_FATP_AUTO_CAST_CUSTOM_TYPES(...)

#define _STC_FATP_AUTO_CAST_INITIALISER(from_transform, to_transform, data, \
                                        expr)                               \
    __STC_FATP_AUTO_CAST(from_transform, to_transform, data, expr, , )

#define _STC_FATP_AUTO_CAST_VAL_TO_PTR(from_transform, to_transform, data, \
                                       expr)                               \
    __STC_FATP_AUTO_CAST(from_transform, to_transform, data, expr, *, &)

#define _STC_FATP_AUTO_CAST_PTR_TO_PTR(from_transform, to_transform, data, \
                                       expr)                               \
    __STC_FATP_AUTO_CAST(from_transform, to_transform, data, expr, *, )

#define __STC_FATP_AUTO_CAST(from_transform, to_transform, data, expr, pntr, \
                             unop)                                           \
    _Generic((data) __STC_FATP_AUTO_CAST_BUILTINS(                           \
        from_transform, to_transform, expr, pntr, unop)                      \
                 __STC_FATP_AUTO_CAST_STDC99(from_transform, to_transform,   \
                                             expr, pntr, unop)               \
                     _STC_FATP_AUTO_CAST_CUSTOM_TYPES(                       \
                         from_transform, to_transform, expr, pntr, unop))

#define __STC_FATP_AUTO_CAST_BUILTINS(from_transform, to_transform, expr,      \
                                      pntr, unop)                              \
    ,                                                                          \
        _STC_FATP_AUTO_CAST_TYPE(void, from_transform, to_transform, expr,     \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(void ptr, from_transform, to_transform, expr, \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const void ptr, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(void ptr const, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const void ptr const, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(signed char, from_transform, to_transform,    \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char, from_transform, to_transform,  \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(char, from_transform, to_transform, expr,     \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const signed char, from_transform,            \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const char, from_transform, to_transform,     \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr, from_transform,              \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr, from_transform,            \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(char ptr, from_transform, to_transform, expr, \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr, from_transform,      \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr const, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr const, from_transform,      \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(char ptr const, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr const, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr const,                \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr const, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr ptr, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr ptr, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(char ptr ptr, from_transform, to_transform,   \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr ptr, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr ptr, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr const ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr const ptr, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(char ptr const ptr, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr const ptr,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr const ptr,            \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr const ptr, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr ptr const, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr ptr const, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(char ptr ptr const, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr ptr const,            \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr ptr const, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed char ptr const ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned char ptr const ptr const,            \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(char ptr const ptr const, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed char ptr const ptr const,        \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned char ptr const ptr const,      \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const char ptr const ptr const,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(signed short int, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr, from_transform, \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr const, from_transform, \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr const,           \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr ptr, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr ptr,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr ptr,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr const ptr,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr const ptr,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr const ptr,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr const ptr,       \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr ptr const,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr ptr const,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr ptr const,       \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed short int ptr const ptr const,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned short int ptr const ptr const,       \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed short int ptr const ptr const,   \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned short int ptr const ptr const, \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(signed int, from_transform, to_transform,     \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int, from_transform, to_transform,   \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const signed int, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr const, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr const, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr const, from_transform, \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr ptr, from_transform,           \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr ptr, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr const ptr, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr const ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr const ptr,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr const ptr,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr ptr const, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr ptr const,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed int ptr const ptr const,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned int ptr const ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed int ptr const ptr const,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned int ptr const ptr const,       \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(signed long int, from_transform,              \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int, from_transform,            \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int, from_transform,      \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr const, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr const, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr const,            \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr ptr, from_transform,      \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr ptr,                \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr ptr,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr const ptr,                \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr const ptr,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr const ptr,          \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr const ptr,        \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr ptr const,                \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr ptr const,          \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr ptr const,        \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long int ptr const ptr const,          \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long int ptr const ptr const,        \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long int ptr const ptr const,    \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long int ptr const ptr const,  \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int, from_transform, \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr, from_transform,     \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int ptr,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int ptr,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr const,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int ptr const,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int ptr const,       \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr ptr, from_transform, \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr ptr,               \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int ptr ptr,           \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int ptr ptr,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr const ptr,           \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr const ptr,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int ptr const ptr,     \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int ptr const ptr,   \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr ptr const,           \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr ptr const,         \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const signed long long int ptr ptr const,     \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const unsigned long long int ptr ptr const,   \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(signed long long int ptr const ptr const,     \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(unsigned long long int ptr const ptr const,   \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(                                              \
            const signed long long int ptr const ptr const, from_transform,    \
            to_transform, expr, pntr, unop),                                   \
        _STC_FATP_AUTO_CAST_TYPE(                                              \
            const unsigned long long int ptr const ptr const, from_transform,  \
            to_transform, expr, pntr, unop),                                   \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(float, from_transform, to_transform, expr,    \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const float, from_transform, to_transform,    \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(float ptr, from_transform, to_transform,      \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr, from_transform,              \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(float ptr const, from_transform,              \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr const, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(float ptr ptr, from_transform, to_transform,  \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr ptr, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(float ptr const ptr, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr const ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(float ptr ptr const, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr ptr const, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(float ptr const ptr const, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const float ptr const ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(double, from_transform, to_transform, expr,   \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const double, from_transform, to_transform,   \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(double ptr, from_transform, to_transform,     \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(double ptr const, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr const, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(double ptr ptr, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(double ptr const ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr const ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(double ptr ptr const, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(double ptr const ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const double ptr const ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(long double, from_transform, to_transform,    \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const long double, from_transform,            \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr, from_transform,              \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr const, from_transform,        \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr const, from_transform,  \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr ptr, from_transform,          \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr const ptr, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr const ptr,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr ptr const, from_transform,    \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(long double ptr const ptr const,              \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
        _STC_FATP_AUTO_CAST_TYPE(const long double ptr const ptr const,        \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop),                                        \
                                                                               \
        _STC_FATP_AUTO_CAST_TYPE(size_t, from_transform, to_transform, expr,   \
                                 pntr, unop),                                  \
        _STC_FATP_AUTO_CAST_TYPE(const size_t, from_transform, to_transform,   \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr, from_transform, to_transform,     \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr const, from_transform,             \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr const, from_transform,       \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr ptr, from_transform, to_transform, \
                                 expr, pntr, unop),                            \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr const ptr, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr const ptr, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr ptr const, from_transform,         \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(size_t ptr const ptr const, from_transform,   \
                                 to_transform, expr, pntr, unop),              \
        _STC_FATP_AUTO_CAST_TYPE(const size_t ptr const ptr const,             \
                                 from_transform, to_transform, expr, pntr,     \
                                 unop)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    define __STC_FATP_AUTO_CAST_STDC99(from_transform, to_transform, expr,    \
                                        pntr, unop)                            \
        ,                                                                      \
            _STC_FATP_AUTO_CAST_TYPE(int8_t, from_transform, to_transform,     \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t, from_transform,             \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr, from_transform, to_transform, \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr const, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr const, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr ptr, from_transform,           \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr ptr, from_transform,     \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr const ptr, from_transform,     \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr const ptr,               \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr ptr const, from_transform,     \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr ptr const,               \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int8_t ptr const ptr const,               \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const int8_t ptr const ptr const,         \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(int16_t, from_transform, to_transform,    \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t, from_transform,            \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr, from_transform,              \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr const, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr const, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr ptr, from_transform,          \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr const ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr const ptr,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr ptr const, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int16_t ptr const ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const int16_t ptr const ptr const,        \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(int32_t, from_transform, to_transform,    \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t, from_transform,            \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr, from_transform,              \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr const, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr const, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr ptr, from_transform,          \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr const ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr const ptr,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr ptr const, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int32_t ptr const ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const int32_t ptr const ptr const,        \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(int64_t, from_transform, to_transform,    \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t, from_transform,            \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr, from_transform,              \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr const, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr const, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr ptr, from_transform,          \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr const ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr const ptr,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr ptr const, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(int64_t ptr const ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const int64_t ptr const ptr const,        \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t, from_transform, to_transform,   \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t, from_transform,           \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr, from_transform,             \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr const, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr const, from_transform, \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr ptr, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr const ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr const ptr,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr ptr const, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(intmax_t ptr const ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const intmax_t ptr const ptr const,       \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t, from_transform, to_transform,    \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t, from_transform,            \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr, from_transform,              \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr const, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr const, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr ptr, from_transform,          \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr const ptr, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr const ptr,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr ptr const, from_transform,    \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint8_t ptr const ptr const,              \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const uint8_t ptr const ptr const,        \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t, from_transform, to_transform,   \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t, from_transform,           \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr, from_transform,             \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr const, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr const, from_transform, \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr ptr, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr const ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr const ptr,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr ptr const, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint16_t ptr const ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const uint16_t ptr const ptr const,       \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t, from_transform, to_transform,   \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t, from_transform,           \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr, from_transform,             \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr const, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr const, from_transform, \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr ptr, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr const ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr const ptr,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr ptr const, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint32_t ptr const ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const uint32_t ptr const ptr const,       \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t, from_transform, to_transform,   \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t, from_transform,           \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr, from_transform,             \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr const, from_transform,       \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr const, from_transform, \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr ptr, from_transform,         \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr const ptr, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr const ptr,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr ptr const, from_transform,   \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uint64_t ptr const ptr const,             \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const uint64_t ptr const ptr const,       \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
                                                                               \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t, from_transform, to_transform,  \
                                     expr, pntr, unop),                        \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t, from_transform,          \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr, from_transform,            \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr, from_transform,      \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr const, from_transform,      \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr const,                \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr ptr, from_transform,        \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr ptr, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr const ptr, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr const ptr,            \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr ptr const, from_transform,  \
                                     to_transform, expr, pntr, unop),          \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr ptr const,            \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(uintmax_t ptr const ptr const,            \
                                     from_transform, to_transform, expr, pntr, \
                                     unop),                                    \
            _STC_FATP_AUTO_CAST_TYPE(const uintmax_t ptr const ptr const,      \
                                     from_transform, to_transform, expr, pntr, \
                                     unop)
#else
#    define __STC_FATP_AUTO_CAST_STDC99(...)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

/* --- Macros to define fat-pointers for builtin types
 * ---------------------- */

#define _STC_FATP_DEFINE_FOR_BUILTIN_TYPES(StcFatp, stc_fatp_define_for)       \
    stc_fatp_define_for(void ptr);                                             \
                                                                               \
    stc_fatp_define_for(const void ptr);                                       \
    typedef StcFatp(const void ptr) StcFatp(void const ptr);                   \
    stc_fatp_define_for(void ptr const);                                       \
    stc_fatp_define_for(const void ptr const);                                 \
    typedef StcFatp(const void ptr const) StcFatp(void const ptr const);       \
                                                                               \
    stc_fatp_define_for(signed char);                                          \
    stc_fatp_define_for(unsigned char);                                        \
    stc_fatp_define_for(char);                                                 \
                                                                               \
    stc_fatp_define_for(const signed char);                                    \
    typedef StcFatp(const signed char) StcFatp(signed const char);             \
    typedef StcFatp(const signed char) StcFatp(signed char const);             \
                                                                               \
    stc_fatp_define_for(const unsigned char);                                  \
    typedef StcFatp(const unsigned char) StcFatp(unsigned const char);         \
    typedef StcFatp(const unsigned char) StcFatp(unsigned char const);         \
                                                                               \
    stc_fatp_define_for(const char);                                           \
    typedef StcFatp(const char) StcFatp(char const);                           \
                                                                               \
    stc_fatp_define_for(signed char ptr);                                      \
    stc_fatp_define_for(unsigned char ptr);                                    \
    stc_fatp_define_for(char ptr);                                             \
                                                                               \
    stc_fatp_define_for(const signed char ptr);                                \
    typedef StcFatp(const signed char ptr) StcFatp(signed const char ptr);     \
    typedef StcFatp(const signed char ptr) StcFatp(signed char const ptr);     \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr);                              \
    typedef StcFatp(const unsigned char ptr) StcFatp(unsigned const char ptr); \
    typedef StcFatp(const unsigned char ptr) StcFatp(unsigned char const ptr); \
                                                                               \
    stc_fatp_define_for(const char ptr);                                       \
    typedef StcFatp(const char ptr) StcFatp(char const ptr);                   \
                                                                               \
    stc_fatp_define_for(signed char ptr const);                                \
    stc_fatp_define_for(unsigned char ptr const);                              \
    stc_fatp_define_for(char ptr const);                                       \
                                                                               \
    stc_fatp_define_for(const signed char ptr const);                          \
    typedef StcFatp(const signed char ptr const)                               \
        StcFatp(signed const char ptr const);                                  \
    typedef StcFatp(const signed char ptr const)                               \
        StcFatp(signed char const ptr const);                                  \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr const);                        \
    typedef StcFatp(const unsigned char ptr const)                             \
        StcFatp(unsigned const char ptr const);                                \
    typedef StcFatp(const unsigned char ptr const)                             \
        StcFatp(unsigned char const ptr const);                                \
                                                                               \
    stc_fatp_define_for(const char ptr const);                                 \
    typedef StcFatp(const char ptr const) StcFatp(char const ptr const);       \
                                                                               \
    stc_fatp_define_for(signed char ptr ptr);                                  \
    stc_fatp_define_for(unsigned char ptr ptr);                                \
    stc_fatp_define_for(char ptr ptr);                                         \
                                                                               \
    stc_fatp_define_for(const signed char ptr ptr);                            \
    typedef StcFatp(const signed char ptr ptr)                                 \
        StcFatp(signed const char ptr ptr);                                    \
    typedef StcFatp(const signed char ptr ptr)                                 \
        StcFatp(signed char const ptr ptr);                                    \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr ptr);                          \
    typedef StcFatp(const unsigned char ptr ptr)                               \
        StcFatp(unsigned const char ptr ptr);                                  \
    typedef StcFatp(const unsigned char ptr ptr)                               \
        StcFatp(unsigned char const ptr ptr);                                  \
                                                                               \
    stc_fatp_define_for(const char ptr ptr);                                   \
    typedef StcFatp(const char ptr ptr) StcFatp(char const ptr ptr);           \
                                                                               \
    stc_fatp_define_for(signed char ptr const ptr);                            \
    stc_fatp_define_for(unsigned char ptr const ptr);                          \
    stc_fatp_define_for(char ptr const ptr);                                   \
                                                                               \
    stc_fatp_define_for(const signed char ptr const ptr);                      \
    typedef StcFatp(const signed char ptr const ptr)                           \
        StcFatp(signed const char ptr const ptr);                              \
    typedef StcFatp(const signed char ptr const ptr)                           \
        StcFatp(signed char const ptr const ptr);                              \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr const ptr);                    \
    typedef StcFatp(const unsigned char ptr const ptr)                         \
        StcFatp(unsigned const char ptr const ptr);                            \
    typedef StcFatp(const unsigned char ptr const ptr)                         \
        StcFatp(unsigned char const ptr const ptr);                            \
                                                                               \
    stc_fatp_define_for(const char ptr const ptr);                             \
    typedef StcFatp(const char ptr const ptr)                                  \
        StcFatp(char const ptr const ptr);                                     \
                                                                               \
    stc_fatp_define_for(signed char ptr ptr const);                            \
    stc_fatp_define_for(unsigned char ptr ptr const);                          \
    stc_fatp_define_for(char ptr ptr const);                                   \
                                                                               \
    stc_fatp_define_for(const signed char ptr ptr const);                      \
    typedef StcFatp(const signed char ptr ptr const)                           \
        StcFatp(signed const char ptr ptr const);                              \
    typedef StcFatp(const signed char ptr ptr const)                           \
        StcFatp(signed char const ptr ptr const);                              \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr ptr const);                    \
    typedef StcFatp(const unsigned char ptr ptr const)                         \
        StcFatp(unsigned const char ptr ptr const);                            \
    typedef StcFatp(const unsigned char ptr ptr const)                         \
        StcFatp(unsigned char const ptr ptr const);                            \
                                                                               \
    stc_fatp_define_for(const char ptr ptr const);                             \
    typedef StcFatp(const char ptr ptr const)                                  \
        StcFatp(char const ptr ptr const);                                     \
                                                                               \
    stc_fatp_define_for(signed char ptr const ptr const);                      \
    stc_fatp_define_for(unsigned char ptr const ptr const);                    \
    stc_fatp_define_for(char ptr const ptr const);                             \
                                                                               \
    stc_fatp_define_for(const signed char ptr const ptr const);                \
    typedef StcFatp(const signed char ptr const ptr const)                     \
        StcFatp(signed const char ptr const ptr const);                        \
    typedef StcFatp(const signed char ptr const ptr const)                     \
        StcFatp(signed char const ptr const ptr const);                        \
                                                                               \
    stc_fatp_define_for(const unsigned char ptr const ptr const);              \
    typedef StcFatp(const unsigned char ptr const ptr const)                   \
        StcFatp(unsigned const char ptr const ptr const);                      \
    typedef StcFatp(const unsigned char ptr const ptr const)                   \
        StcFatp(unsigned char const ptr const ptr const);                      \
                                                                               \
    stc_fatp_define_for(const char ptr const ptr const);                       \
    typedef StcFatp(const char ptr const ptr const)                            \
        StcFatp(char const ptr const ptr const);                               \
                                                                               \
    stc_fatp_define_for(signed short int);                                     \
    typedef StcFatp(signed short int) StcFatp(signed short);                   \
    typedef StcFatp(signed short int) StcFatp(short int);                      \
    typedef StcFatp(signed short) StcFatp(short);                              \
                                                                               \
    stc_fatp_define_for(unsigned short int);                                   \
    typedef StcFatp(unsigned short int) StcFatp(unsigned short);               \
                                                                               \
    stc_fatp_define_for(const signed short int);                               \
    typedef StcFatp(const signed short int) StcFatp(signed const short int);   \
    typedef StcFatp(const signed short int) StcFatp(signed short const int);   \
    typedef StcFatp(const signed short int) StcFatp(signed short int const);   \
    typedef StcFatp(const signed short int) StcFatp(const signed short);       \
    typedef StcFatp(const signed short) StcFatp(signed const short);           \
    typedef StcFatp(const signed short) StcFatp(signed short const);           \
    typedef StcFatp(const signed short int) StcFatp(const short int);          \
    typedef StcFatp(const short int) StcFatp(short const int);                 \
    typedef StcFatp(const short int) StcFatp(short int const);                 \
    typedef StcFatp(const signed short) StcFatp(const short);                  \
    typedef StcFatp(const short) StcFatp(short const);                         \
                                                                               \
    stc_fatp_define_for(const unsigned short int);                             \
    typedef StcFatp(const unsigned short int)                                  \
        StcFatp(unsigned const short int);                                     \
    typedef StcFatp(const unsigned short int)                                  \
        StcFatp(unsigned short const int);                                     \
    typedef StcFatp(const unsigned short int)                                  \
        StcFatp(unsigned short int const);                                     \
    typedef StcFatp(const unsigned short int) StcFatp(const unsigned short);   \
    typedef StcFatp(const unsigned short) StcFatp(unsigned const short);       \
    typedef StcFatp(const unsigned short) StcFatp(unsigned short const);       \
                                                                               \
    stc_fatp_define_for(signed short int ptr);                                 \
    typedef StcFatp(signed short int ptr) StcFatp(signed short ptr);           \
    typedef StcFatp(signed short int ptr) StcFatp(short int ptr);              \
    typedef StcFatp(signed short ptr) StcFatp(short ptr);                      \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr);                               \
    typedef StcFatp(unsigned short int ptr) StcFatp(unsigned short ptr);       \
                                                                               \
    stc_fatp_define_for(const signed short int ptr);                           \
    typedef StcFatp(const signed short int ptr)                                \
        StcFatp(signed const short int ptr);                                   \
    typedef StcFatp(const signed short int ptr)                                \
        StcFatp(signed short const int ptr);                                   \
    typedef StcFatp(const signed short int ptr)                                \
        StcFatp(signed short int const ptr);                                   \
    typedef StcFatp(const signed short int ptr)                                \
        StcFatp(const signed short ptr);                                       \
    typedef StcFatp(const signed short ptr) StcFatp(signed const short ptr);   \
    typedef StcFatp(const signed short ptr) StcFatp(signed short const ptr);   \
    typedef StcFatp(const signed short int ptr) StcFatp(const short int ptr);  \
    typedef StcFatp(const short int ptr) StcFatp(short const int ptr);         \
    typedef StcFatp(const short int ptr) StcFatp(short int const ptr);         \
    typedef StcFatp(const signed short ptr) StcFatp(const short ptr);          \
    typedef StcFatp(const short ptr) StcFatp(short const ptr);                 \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr);                         \
    typedef StcFatp(const unsigned short int ptr)                              \
        StcFatp(unsigned const short int ptr);                                 \
    typedef StcFatp(const unsigned short int ptr)                              \
        StcFatp(unsigned short const int ptr);                                 \
    typedef StcFatp(const unsigned short int ptr)                              \
        StcFatp(unsigned short int const ptr);                                 \
    typedef StcFatp(const unsigned short int ptr)                              \
        StcFatp(const unsigned short ptr);                                     \
    typedef StcFatp(const unsigned short ptr)                                  \
        StcFatp(unsigned const short ptr);                                     \
    typedef StcFatp(const unsigned short ptr)                                  \
        StcFatp(unsigned short const ptr);                                     \
                                                                               \
    stc_fatp_define_for(signed short int ptr const);                           \
    typedef StcFatp(signed short int ptr const)                                \
        StcFatp(signed short ptr const);                                       \
    typedef StcFatp(signed short int ptr const) StcFatp(short int ptr const);  \
    typedef StcFatp(signed short ptr const) StcFatp(short ptr const);          \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr const);                         \
    typedef StcFatp(unsigned short int ptr const)                              \
        StcFatp(unsigned short ptr const);                                     \
                                                                               \
    stc_fatp_define_for(const signed short int ptr const);                     \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(signed const short int ptr const);                             \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(signed short const int ptr const);                             \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(signed short int const ptr const);                             \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(const signed short ptr const);                                 \
    typedef StcFatp(const signed short ptr const)                              \
        StcFatp(signed const short ptr const);                                 \
    typedef StcFatp(const signed short ptr const)                              \
        StcFatp(signed short const ptr const);                                 \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(const short int ptr const);                                    \
    typedef StcFatp(const short int ptr const)                                 \
        StcFatp(short const int ptr const);                                    \
    typedef StcFatp(const short int ptr const)                                 \
        StcFatp(short int const ptr const);                                    \
    typedef StcFatp(const signed short ptr const)                              \
        StcFatp(const short ptr const);                                        \
    typedef StcFatp(const short ptr const) StcFatp(short const ptr const);     \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr const);                   \
    typedef StcFatp(const unsigned short int ptr const)                        \
        StcFatp(unsigned const short int ptr const);                           \
    typedef StcFatp(const unsigned short int ptr const)                        \
        StcFatp(unsigned short const int ptr const);                           \
    typedef StcFatp(const unsigned short int ptr const)                        \
        StcFatp(unsigned short int const ptr const);                           \
    typedef StcFatp(const unsigned short int ptr const)                        \
        StcFatp(const unsigned short ptr const);                               \
    typedef StcFatp(const unsigned short ptr const)                            \
        StcFatp(unsigned const short ptr const);                               \
    typedef StcFatp(const unsigned short ptr const)                            \
        StcFatp(unsigned short const ptr const);                               \
                                                                               \
    stc_fatp_define_for(signed short int ptr ptr);                             \
    typedef StcFatp(signed short int ptr ptr) StcFatp(signed short ptr ptr);   \
    typedef StcFatp(signed short int ptr ptr) StcFatp(short int ptr ptr);      \
    typedef StcFatp(signed short ptr ptr) StcFatp(short ptr ptr);              \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr ptr);                           \
    typedef StcFatp(unsigned short int ptr ptr)                                \
        StcFatp(unsigned short ptr ptr);                                       \
                                                                               \
    stc_fatp_define_for(const signed short int ptr ptr);                       \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(signed const short int ptr ptr);                               \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(signed short const int ptr ptr);                               \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(signed short int const ptr ptr);                               \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(const signed short ptr ptr);                                   \
    typedef StcFatp(const signed short ptr ptr)                                \
        StcFatp(signed const short ptr ptr);                                   \
    typedef StcFatp(const signed short ptr ptr)                                \
        StcFatp(signed short const ptr ptr);                                   \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(const short int ptr ptr);                                      \
    typedef StcFatp(const short int ptr ptr) StcFatp(short const int ptr ptr); \
    typedef StcFatp(const short int ptr ptr) StcFatp(short int const ptr ptr); \
    typedef StcFatp(const signed short ptr ptr) StcFatp(const short ptr ptr);  \
    typedef StcFatp(const short ptr ptr) StcFatp(short const ptr ptr);         \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr ptr);                     \
    typedef StcFatp(const unsigned short int ptr ptr)                          \
        StcFatp(unsigned const short int ptr ptr);                             \
    typedef StcFatp(const unsigned short int ptr ptr)                          \
        StcFatp(unsigned short const int ptr ptr);                             \
    typedef StcFatp(const unsigned short int ptr ptr)                          \
        StcFatp(unsigned short int const ptr ptr);                             \
    typedef StcFatp(const unsigned short int ptr ptr)                          \
        StcFatp(const unsigned short ptr ptr);                                 \
    typedef StcFatp(const unsigned short ptr ptr)                              \
        StcFatp(unsigned const short ptr ptr);                                 \
    typedef StcFatp(const unsigned short ptr ptr)                              \
        StcFatp(unsigned short const ptr ptr);                                 \
                                                                               \
    stc_fatp_define_for(signed short int ptr const ptr);                       \
    typedef StcFatp(signed short int ptr const ptr)                            \
        StcFatp(signed short ptr const ptr);                                   \
    typedef StcFatp(signed short int ptr const ptr)                            \
        StcFatp(short int ptr const ptr);                                      \
    typedef StcFatp(signed short ptr const ptr) StcFatp(short ptr const ptr);  \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr const ptr);                     \
    typedef StcFatp(unsigned short int ptr const ptr)                          \
        StcFatp(unsigned short ptr const ptr);                                 \
                                                                               \
    stc_fatp_define_for(const signed short int ptr const ptr);                 \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(signed const short int ptr const ptr);                         \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(signed short const int ptr const ptr);                         \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(signed short int const ptr const ptr);                         \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(const signed short ptr const ptr);                             \
    typedef StcFatp(const signed short ptr const ptr)                          \
        StcFatp(signed const short ptr const ptr);                             \
    typedef StcFatp(const signed short ptr const ptr)                          \
        StcFatp(signed short const ptr const ptr);                             \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(const short int ptr const ptr);                                \
    typedef StcFatp(const short int ptr const ptr)                             \
        StcFatp(short const int ptr const ptr);                                \
    typedef StcFatp(const short int ptr const ptr)                             \
        StcFatp(short int const ptr const ptr);                                \
    typedef StcFatp(const signed short ptr const ptr)                          \
        StcFatp(const short ptr const ptr);                                    \
    typedef StcFatp(const short ptr const ptr)                                 \
        StcFatp(short const ptr const ptr);                                    \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr const ptr);               \
    typedef StcFatp(const unsigned short int ptr const ptr)                    \
        StcFatp(unsigned const short int ptr const ptr);                       \
    typedef StcFatp(const unsigned short int ptr const ptr)                    \
        StcFatp(unsigned short const int ptr const ptr);                       \
    typedef StcFatp(const unsigned short int ptr const ptr)                    \
        StcFatp(unsigned short int const ptr const ptr);                       \
    typedef StcFatp(const unsigned short int ptr const ptr)                    \
        StcFatp(const unsigned short ptr const ptr);                           \
    typedef StcFatp(const unsigned short ptr const ptr)                        \
        StcFatp(unsigned const short ptr const ptr);                           \
    typedef StcFatp(const unsigned short ptr const ptr)                        \
        StcFatp(unsigned short const ptr const ptr);                           \
                                                                               \
    stc_fatp_define_for(signed short int ptr ptr const);                       \
    typedef StcFatp(signed short int ptr ptr const)                            \
        StcFatp(signed short ptr ptr const);                                   \
    typedef StcFatp(signed short int ptr ptr const)                            \
        StcFatp(short int ptr ptr const);                                      \
    typedef StcFatp(signed short ptr ptr const) StcFatp(short ptr ptr const);  \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr ptr const);                     \
    typedef StcFatp(unsigned short int ptr ptr const)                          \
        StcFatp(unsigned short ptr ptr const);                                 \
                                                                               \
    stc_fatp_define_for(const signed short int ptr ptr const);                 \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(signed const short int ptr ptr const);                         \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(signed short const int ptr ptr const);                         \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(signed short int const ptr ptr const);                         \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(const signed short ptr ptr const);                             \
    typedef StcFatp(const signed short ptr ptr const)                          \
        StcFatp(signed const short ptr ptr const);                             \
    typedef StcFatp(const signed short ptr ptr const)                          \
        StcFatp(signed short const ptr ptr const);                             \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(const short int ptr ptr const);                                \
    typedef StcFatp(const short int ptr ptr const)                             \
        StcFatp(short const int ptr ptr const);                                \
    typedef StcFatp(const short int ptr ptr const)                             \
        StcFatp(short int const ptr ptr const);                                \
    typedef StcFatp(const signed short ptr ptr const)                          \
        StcFatp(const short ptr ptr const);                                    \
    typedef StcFatp(const short ptr ptr const)                                 \
        StcFatp(short const ptr ptr const);                                    \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr ptr const);               \
    typedef StcFatp(const unsigned short int ptr ptr const)                    \
        StcFatp(unsigned const short int ptr ptr const);                       \
    typedef StcFatp(const unsigned short int ptr ptr const)                    \
        StcFatp(unsigned short const int ptr ptr const);                       \
    typedef StcFatp(const unsigned short int ptr ptr const)                    \
        StcFatp(unsigned short int const ptr ptr const);                       \
    typedef StcFatp(const unsigned short int ptr ptr const)                    \
        StcFatp(const unsigned short ptr ptr const);                           \
    typedef StcFatp(const unsigned short ptr ptr const)                        \
        StcFatp(unsigned const short ptr ptr const);                           \
    typedef StcFatp(const unsigned short ptr ptr const)                        \
        StcFatp(unsigned short const ptr ptr const);                           \
                                                                               \
    stc_fatp_define_for(signed short int ptr const ptr const);                 \
    typedef StcFatp(signed short int ptr const ptr const)                      \
        StcFatp(signed short ptr const ptr const);                             \
    typedef StcFatp(signed short int ptr const ptr const)                      \
        StcFatp(short int ptr const ptr const);                                \
    typedef StcFatp(signed short ptr const ptr const)                          \
        StcFatp(short ptr const ptr const);                                    \
                                                                               \
    stc_fatp_define_for(unsigned short int ptr const ptr const);               \
    typedef StcFatp(unsigned short int ptr const ptr const)                    \
        StcFatp(unsigned short ptr const ptr const);                           \
                                                                               \
    stc_fatp_define_for(const signed short int ptr const ptr const);           \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(signed const short int ptr const ptr const);                   \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(signed short const int ptr const ptr const);                   \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(signed short int const ptr const ptr const);                   \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(const signed short ptr const ptr const);                       \
    typedef StcFatp(const signed short ptr const ptr const)                    \
        StcFatp(signed const short ptr const ptr const);                       \
    typedef StcFatp(const signed short ptr const ptr const)                    \
        StcFatp(signed short const ptr const ptr const);                       \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(const short int ptr const ptr const);                          \
    typedef StcFatp(const short int ptr const ptr const)                       \
        StcFatp(short const int ptr const ptr const);                          \
    typedef StcFatp(const short int ptr const ptr const)                       \
        StcFatp(short int const ptr const ptr const);                          \
    typedef StcFatp(const signed short ptr const ptr const)                    \
        StcFatp(const short ptr const ptr const);                              \
    typedef StcFatp(const short ptr const ptr const)                           \
        StcFatp(short const ptr const ptr const);                              \
                                                                               \
    stc_fatp_define_for(const unsigned short int ptr const ptr const);         \
    typedef StcFatp(const unsigned short int ptr const ptr const)              \
        StcFatp(unsigned const short int ptr const ptr const);                 \
    typedef StcFatp(const unsigned short int ptr const ptr const)              \
        StcFatp(unsigned short const int ptr const ptr const);                 \
    typedef StcFatp(const unsigned short int ptr const ptr const)              \
        StcFatp(unsigned short int const ptr const ptr const);                 \
    typedef StcFatp(const unsigned short int ptr const ptr const)              \
        StcFatp(const unsigned short ptr const ptr const);                     \
    typedef StcFatp(const unsigned short ptr const ptr const)                  \
        StcFatp(unsigned const short ptr const ptr const);                     \
    typedef StcFatp(const unsigned short ptr const ptr const)                  \
        StcFatp(unsigned short const ptr const ptr const);                     \
                                                                               \
    stc_fatp_define_for(signed int);                                           \
    typedef StcFatp(signed int) StcFatp(signed);                               \
    typedef StcFatp(signed int) StcFatp(int);                                  \
                                                                               \
    stc_fatp_define_for(unsigned int);                                         \
    typedef StcFatp(unsigned int) StcFatp(unsigned);                           \
                                                                               \
    stc_fatp_define_for(const signed int);                                     \
    typedef StcFatp(const signed int) StcFatp(signed const int);               \
    typedef StcFatp(const signed int) StcFatp(signed int const);               \
    typedef StcFatp(const signed int) StcFatp(const signed);                   \
    typedef StcFatp(const signed) StcFatp(signed const);                       \
    typedef StcFatp(const signed int) StcFatp(const int);                      \
    typedef StcFatp(const int) StcFatp(int const);                             \
                                                                               \
    stc_fatp_define_for(const unsigned int);                                   \
    typedef StcFatp(const unsigned int) StcFatp(unsigned const int);           \
    typedef StcFatp(const unsigned int) StcFatp(unsigned int const);           \
    typedef StcFatp(const unsigned int) StcFatp(const unsigned);               \
    typedef StcFatp(const unsigned) StcFatp(unsigned const);                   \
                                                                               \
    stc_fatp_define_for(signed int ptr);                                       \
    typedef StcFatp(signed int ptr) StcFatp(signed ptr);                       \
    typedef StcFatp(signed int ptr) StcFatp(int ptr);                          \
                                                                               \
    stc_fatp_define_for(unsigned int ptr);                                     \
    typedef StcFatp(unsigned int ptr) StcFatp(unsigned ptr);                   \
                                                                               \
    stc_fatp_define_for(const signed int ptr);                                 \
    typedef StcFatp(const signed int ptr) StcFatp(signed const int ptr);       \
    typedef StcFatp(const signed int ptr) StcFatp(signed int const ptr);       \
    typedef StcFatp(const signed int ptr) StcFatp(const signed ptr);           \
    typedef StcFatp(const signed ptr) StcFatp(signed const ptr);               \
    typedef StcFatp(const signed int ptr) StcFatp(const int ptr);              \
    typedef StcFatp(const int ptr) StcFatp(int const ptr);                     \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr);                               \
    typedef StcFatp(const unsigned int ptr) StcFatp(unsigned const int ptr);   \
    typedef StcFatp(const unsigned int ptr) StcFatp(unsigned int const ptr);   \
    typedef StcFatp(const unsigned int ptr) StcFatp(const unsigned ptr);       \
    typedef StcFatp(const unsigned ptr) StcFatp(unsigned const ptr);           \
                                                                               \
    stc_fatp_define_for(signed int ptr const);                                 \
    typedef StcFatp(signed int ptr const) StcFatp(signed ptr const);           \
    typedef StcFatp(signed int ptr const) StcFatp(int ptr const);              \
                                                                               \
    stc_fatp_define_for(unsigned int ptr const);                               \
    typedef StcFatp(unsigned int ptr const) StcFatp(unsigned ptr const);       \
                                                                               \
    stc_fatp_define_for(const signed int ptr const);                           \
    typedef StcFatp(const signed int ptr const)                                \
        StcFatp(signed const int ptr const);                                   \
    typedef StcFatp(const signed int ptr const)                                \
        StcFatp(signed int const ptr const);                                   \
    typedef StcFatp(const signed int ptr const)                                \
        StcFatp(const signed ptr const);                                       \
    typedef StcFatp(const signed ptr const) StcFatp(signed const ptr const);   \
    typedef StcFatp(const signed int ptr const) StcFatp(const int ptr const);  \
    typedef StcFatp(const int ptr const) StcFatp(int const ptr const);         \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr const);                         \
    typedef StcFatp(const unsigned int ptr const)                              \
        StcFatp(unsigned const int ptr const);                                 \
    typedef StcFatp(const unsigned int ptr const)                              \
        StcFatp(unsigned int const ptr const);                                 \
    typedef StcFatp(const unsigned int ptr const)                              \
        StcFatp(const unsigned ptr const);                                     \
    typedef StcFatp(const unsigned ptr const)                                  \
        StcFatp(unsigned const ptr const);                                     \
                                                                               \
    stc_fatp_define_for(signed int ptr ptr);                                   \
    typedef StcFatp(signed int ptr ptr) StcFatp(signed ptr ptr);               \
    typedef StcFatp(signed int ptr ptr) StcFatp(int ptr ptr);                  \
                                                                               \
    stc_fatp_define_for(unsigned int ptr ptr);                                 \
    typedef StcFatp(unsigned int ptr ptr) StcFatp(unsigned ptr ptr);           \
                                                                               \
    stc_fatp_define_for(const signed int ptr ptr);                             \
    typedef StcFatp(const signed int ptr ptr)                                  \
        StcFatp(signed const int ptr ptr);                                     \
    typedef StcFatp(const signed int ptr ptr)                                  \
        StcFatp(signed int const ptr ptr);                                     \
    typedef StcFatp(const signed int ptr ptr) StcFatp(const signed ptr ptr);   \
    typedef StcFatp(const signed ptr ptr) StcFatp(signed const ptr ptr);       \
    typedef StcFatp(const signed int ptr ptr) StcFatp(const int ptr ptr);      \
    typedef StcFatp(const int ptr ptr) StcFatp(int const ptr ptr);             \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr ptr);                           \
    typedef StcFatp(const unsigned int ptr ptr)                                \
        StcFatp(unsigned const int ptr ptr);                                   \
    typedef StcFatp(const unsigned int ptr ptr)                                \
        StcFatp(unsigned int const ptr ptr);                                   \
    typedef StcFatp(const unsigned int ptr ptr)                                \
        StcFatp(const unsigned ptr ptr);                                       \
    typedef StcFatp(const unsigned ptr ptr) StcFatp(unsigned const ptr ptr);   \
                                                                               \
    stc_fatp_define_for(signed int ptr const ptr);                             \
    typedef StcFatp(signed int ptr const ptr) StcFatp(signed ptr const ptr);   \
    typedef StcFatp(signed int ptr const ptr) StcFatp(int ptr const ptr);      \
                                                                               \
    stc_fatp_define_for(unsigned int ptr const ptr);                           \
    typedef StcFatp(unsigned int ptr const ptr)                                \
        StcFatp(unsigned ptr const ptr);                                       \
                                                                               \
    stc_fatp_define_for(const signed int ptr const ptr);                       \
    typedef StcFatp(const signed int ptr const ptr)                            \
        StcFatp(signed const int ptr const ptr);                               \
    typedef StcFatp(const signed int ptr const ptr)                            \
        StcFatp(signed int const ptr const ptr);                               \
    typedef StcFatp(const signed int ptr const ptr)                            \
        StcFatp(const signed ptr const ptr);                                   \
    typedef StcFatp(const signed ptr const ptr)                                \
        StcFatp(signed const ptr const ptr);                                   \
    typedef StcFatp(const signed int ptr const ptr)                            \
        StcFatp(const int ptr const ptr);                                      \
    typedef StcFatp(const int ptr const ptr) StcFatp(int const ptr const ptr); \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr const ptr);                     \
    typedef StcFatp(const unsigned int ptr const ptr)                          \
        StcFatp(unsigned const int ptr const ptr);                             \
    typedef StcFatp(const unsigned int ptr const ptr)                          \
        StcFatp(unsigned int const ptr const ptr);                             \
    typedef StcFatp(const unsigned int ptr const ptr)                          \
        StcFatp(const unsigned ptr const ptr);                                 \
    typedef StcFatp(const unsigned ptr const ptr)                              \
        StcFatp(unsigned const ptr const ptr);                                 \
                                                                               \
    stc_fatp_define_for(signed int ptr ptr const);                             \
    typedef StcFatp(signed int ptr ptr const) StcFatp(signed ptr ptr const);   \
    typedef StcFatp(signed int ptr ptr const) StcFatp(int ptr ptr const);      \
                                                                               \
    stc_fatp_define_for(unsigned int ptr ptr const);                           \
    typedef StcFatp(unsigned int ptr ptr const)                                \
        StcFatp(unsigned ptr ptr const);                                       \
                                                                               \
    stc_fatp_define_for(const signed int ptr ptr const);                       \
    typedef StcFatp(const signed int ptr ptr const)                            \
        StcFatp(signed const int ptr ptr const);                               \
    typedef StcFatp(const signed int ptr ptr const)                            \
        StcFatp(signed int const ptr ptr const);                               \
    typedef StcFatp(const signed int ptr ptr const)                            \
        StcFatp(const signed ptr ptr const);                                   \
    typedef StcFatp(const signed ptr ptr const)                                \
        StcFatp(signed const ptr ptr const);                                   \
    typedef StcFatp(const signed int ptr ptr const)                            \
        StcFatp(const int ptr ptr const);                                      \
    typedef StcFatp(const int ptr ptr const) StcFatp(int const ptr ptr const); \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr ptr const);                     \
    typedef StcFatp(const unsigned int ptr ptr const)                          \
        StcFatp(unsigned const int ptr ptr const);                             \
    typedef StcFatp(const unsigned int ptr ptr const)                          \
        StcFatp(unsigned int const ptr ptr const);                             \
    typedef StcFatp(const unsigned int ptr ptr const)                          \
        StcFatp(const unsigned ptr ptr const);                                 \
    typedef StcFatp(const unsigned ptr ptr const)                              \
        StcFatp(unsigned const ptr ptr const);                                 \
                                                                               \
    stc_fatp_define_for(signed int ptr const ptr const);                       \
    typedef StcFatp(signed int ptr const ptr const)                            \
        StcFatp(signed ptr const ptr const);                                   \
    typedef StcFatp(signed int ptr const ptr const)                            \
        StcFatp(int ptr const ptr const);                                      \
                                                                               \
    stc_fatp_define_for(unsigned int ptr const ptr const);                     \
    typedef StcFatp(unsigned int ptr const ptr const)                          \
        StcFatp(unsigned ptr const ptr const);                                 \
                                                                               \
    stc_fatp_define_for(const signed int ptr const ptr const);                 \
    typedef StcFatp(const signed int ptr const ptr const)                      \
        StcFatp(signed const int ptr const ptr const);                         \
    typedef StcFatp(const signed int ptr const ptr const)                      \
        StcFatp(signed int const ptr const ptr const);                         \
    typedef StcFatp(const signed int ptr const ptr const)                      \
        StcFatp(const signed ptr const ptr const);                             \
    typedef StcFatp(const signed ptr const ptr const)                          \
        StcFatp(signed const ptr const ptr const);                             \
    typedef StcFatp(const signed int ptr const ptr const)                      \
        StcFatp(const int ptr const ptr const);                                \
    typedef StcFatp(const int ptr const ptr const)                             \
        StcFatp(int const ptr const ptr const);                                \
                                                                               \
    stc_fatp_define_for(const unsigned int ptr const ptr const);               \
    typedef StcFatp(const unsigned int ptr const ptr const)                    \
        StcFatp(unsigned const int ptr const ptr const);                       \
    typedef StcFatp(const unsigned int ptr const ptr const)                    \
        StcFatp(unsigned int const ptr const ptr const);                       \
    typedef StcFatp(const unsigned int ptr const ptr const)                    \
        StcFatp(const unsigned ptr const ptr const);                           \
    typedef StcFatp(const unsigned ptr const ptr const)                        \
        StcFatp(unsigned const ptr const ptr const);                           \
                                                                               \
    stc_fatp_define_for(signed long int);                                      \
    typedef StcFatp(signed long int) StcFatp(signed long);                     \
    typedef StcFatp(signed long int) StcFatp(long int);                        \
    typedef StcFatp(signed long) StcFatp(long);                                \
                                                                               \
    stc_fatp_define_for(unsigned long int);                                    \
    typedef StcFatp(unsigned long int) StcFatp(unsigned long);                 \
                                                                               \
    stc_fatp_define_for(const signed long int);                                \
    typedef StcFatp(const signed long int) StcFatp(signed const long int);     \
    typedef StcFatp(const signed long int) StcFatp(signed long const int);     \
    typedef StcFatp(const signed long int) StcFatp(signed long int const);     \
    typedef StcFatp(const signed long int) StcFatp(const signed long);         \
    typedef StcFatp(const signed long) StcFatp(signed const long);             \
    typedef StcFatp(const signed long) StcFatp(signed long const);             \
    typedef StcFatp(const signed long int) StcFatp(const long int);            \
    typedef StcFatp(const long int) StcFatp(long const int);                   \
    typedef StcFatp(const long int) StcFatp(long int const);                   \
    typedef StcFatp(const signed long) StcFatp(const long);                    \
    typedef StcFatp(const long) StcFatp(long const);                           \
                                                                               \
    stc_fatp_define_for(const unsigned long int);                              \
    typedef StcFatp(const unsigned long int) StcFatp(unsigned const long int); \
    typedef StcFatp(const unsigned long int) StcFatp(unsigned long const int); \
    typedef StcFatp(const unsigned long int) StcFatp(unsigned long int const); \
    typedef StcFatp(const unsigned long int) StcFatp(const unsigned long);     \
    typedef StcFatp(const unsigned long) StcFatp(unsigned const long);         \
    typedef StcFatp(const unsigned long) StcFatp(unsigned long const);         \
                                                                               \
    stc_fatp_define_for(signed long int ptr);                                  \
    typedef StcFatp(signed long int ptr) StcFatp(signed long ptr);             \
    typedef StcFatp(signed long int ptr) StcFatp(long int ptr);                \
    typedef StcFatp(signed long ptr) StcFatp(long ptr);                        \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr);                                \
    typedef StcFatp(unsigned long int ptr) StcFatp(unsigned long ptr);         \
                                                                               \
    stc_fatp_define_for(const signed long int ptr);                            \
    typedef StcFatp(const signed long int ptr)                                 \
        StcFatp(signed const long int ptr);                                    \
    typedef StcFatp(const signed long int ptr)                                 \
        StcFatp(signed long const int ptr);                                    \
    typedef StcFatp(const signed long int ptr)                                 \
        StcFatp(signed long int const ptr);                                    \
    typedef StcFatp(const signed long int ptr) StcFatp(const signed long ptr); \
    typedef StcFatp(const signed long ptr) StcFatp(signed const long ptr);     \
    typedef StcFatp(const signed long ptr) StcFatp(signed long const ptr);     \
    typedef StcFatp(const signed long int ptr) StcFatp(const long int ptr);    \
    typedef StcFatp(const long int ptr) StcFatp(long const int ptr);           \
    typedef StcFatp(const long int ptr) StcFatp(long int const ptr);           \
    typedef StcFatp(const signed long ptr) StcFatp(const long ptr);            \
    typedef StcFatp(const long ptr) StcFatp(long const ptr);                   \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr);                          \
    typedef StcFatp(const unsigned long int ptr)                               \
        StcFatp(unsigned const long int ptr);                                  \
    typedef StcFatp(const unsigned long int ptr)                               \
        StcFatp(unsigned long const int ptr);                                  \
    typedef StcFatp(const unsigned long int ptr)                               \
        StcFatp(unsigned long int const ptr);                                  \
    typedef StcFatp(const unsigned long int ptr)                               \
        StcFatp(const unsigned long ptr);                                      \
    typedef StcFatp(const unsigned long ptr) StcFatp(unsigned const long ptr); \
    typedef StcFatp(const unsigned long ptr) StcFatp(unsigned long const ptr); \
                                                                               \
    stc_fatp_define_for(signed long int ptr const);                            \
    typedef StcFatp(signed long int ptr const) StcFatp(signed long ptr const); \
    typedef StcFatp(signed long int ptr const) StcFatp(long int ptr const);    \
    typedef StcFatp(signed long ptr const) StcFatp(long ptr const);            \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr const);                          \
    typedef StcFatp(unsigned long int ptr const)                               \
        StcFatp(unsigned long ptr const);                                      \
                                                                               \
    stc_fatp_define_for(const signed long int ptr const);                      \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(signed const long int ptr const);                              \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(signed long const int ptr const);                              \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(signed long int const ptr const);                              \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(const signed long ptr const);                                  \
    typedef StcFatp(const signed long ptr const)                               \
        StcFatp(signed const long ptr const);                                  \
    typedef StcFatp(const signed long ptr const)                               \
        StcFatp(signed long const ptr const);                                  \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(const long int ptr const);                                     \
    typedef StcFatp(const long int ptr const)                                  \
        StcFatp(long const int ptr const);                                     \
    typedef StcFatp(const long int ptr const)                                  \
        StcFatp(long int const ptr const);                                     \
    typedef StcFatp(const signed long ptr const)                               \
        StcFatp(const long ptr const);                                         \
    typedef StcFatp(const long ptr const) StcFatp(long const ptr const);       \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr const);                    \
    typedef StcFatp(const unsigned long int ptr const)                         \
        StcFatp(unsigned const long int ptr const);                            \
    typedef StcFatp(const unsigned long int ptr const)                         \
        StcFatp(unsigned long const int ptr const);                            \
    typedef StcFatp(const unsigned long int ptr const)                         \
        StcFatp(unsigned long int const ptr const);                            \
    typedef StcFatp(const unsigned long int ptr const)                         \
        StcFatp(const unsigned long ptr const);                                \
    typedef StcFatp(const unsigned long ptr const)                             \
        StcFatp(unsigned const long ptr const);                                \
    typedef StcFatp(const unsigned long ptr const)                             \
        StcFatp(unsigned long const ptr const);                                \
                                                                               \
    stc_fatp_define_for(signed long int ptr ptr);                              \
    typedef StcFatp(signed long int ptr ptr) StcFatp(signed long ptr ptr);     \
    typedef StcFatp(signed long int ptr ptr) StcFatp(long int ptr ptr);        \
    typedef StcFatp(signed long ptr ptr) StcFatp(long ptr ptr);                \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr ptr);                            \
    typedef StcFatp(unsigned long int ptr ptr) StcFatp(unsigned long ptr ptr); \
                                                                               \
    stc_fatp_define_for(const signed long int ptr ptr);                        \
    typedef StcFatp(const signed long int ptr ptr)                             \
        StcFatp(signed const long int ptr ptr);                                \
    typedef StcFatp(const signed long int ptr ptr)                             \
        StcFatp(signed long const int ptr ptr);                                \
    typedef StcFatp(const signed long int ptr ptr)                             \
        StcFatp(signed long int const ptr ptr);                                \
    typedef StcFatp(const signed long int ptr ptr)                             \
        StcFatp(const signed long ptr ptr);                                    \
    typedef StcFatp(const signed long ptr ptr)                                 \
        StcFatp(signed const long ptr ptr);                                    \
    typedef StcFatp(const signed long ptr ptr)                                 \
        StcFatp(signed long const ptr ptr);                                    \
    typedef StcFatp(const signed long int ptr ptr)                             \
        StcFatp(const long int ptr ptr);                                       \
    typedef StcFatp(const long int ptr ptr) StcFatp(long const int ptr ptr);   \
    typedef StcFatp(const long int ptr ptr) StcFatp(long int const ptr ptr);   \
    typedef StcFatp(const signed long ptr ptr) StcFatp(const long ptr ptr);    \
    typedef StcFatp(const long ptr ptr) StcFatp(long const ptr ptr);           \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr ptr);                      \
    typedef StcFatp(const unsigned long int ptr ptr)                           \
        StcFatp(unsigned const long int ptr ptr);                              \
    typedef StcFatp(const unsigned long int ptr ptr)                           \
        StcFatp(unsigned long const int ptr ptr);                              \
    typedef StcFatp(const unsigned long int ptr ptr)                           \
        StcFatp(unsigned long int const ptr ptr);                              \
    typedef StcFatp(const unsigned long int ptr ptr)                           \
        StcFatp(const unsigned long ptr ptr);                                  \
    typedef StcFatp(const unsigned long ptr ptr)                               \
        StcFatp(unsigned const long ptr ptr);                                  \
    typedef StcFatp(const unsigned long ptr ptr)                               \
        StcFatp(unsigned long const ptr ptr);                                  \
                                                                               \
    stc_fatp_define_for(signed long int ptr const ptr);                        \
    typedef StcFatp(signed long int ptr const ptr)                             \
        StcFatp(signed long ptr const ptr);                                    \
    typedef StcFatp(signed long int ptr const ptr)                             \
        StcFatp(long int ptr const ptr);                                       \
    typedef StcFatp(signed long ptr const ptr) StcFatp(long ptr const ptr);    \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr const ptr);                      \
    typedef StcFatp(unsigned long int ptr const ptr)                           \
        StcFatp(unsigned long ptr const ptr);                                  \
                                                                               \
    stc_fatp_define_for(const signed long int ptr const ptr);                  \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(signed const long int ptr const ptr);                          \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(signed long const int ptr const ptr);                          \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(signed long int const ptr const ptr);                          \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(const signed long ptr const ptr);                              \
    typedef StcFatp(const signed long ptr const ptr)                           \
        StcFatp(signed const long ptr const ptr);                              \
    typedef StcFatp(const signed long ptr const ptr)                           \
        StcFatp(signed long const ptr const ptr);                              \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(const long int ptr const ptr);                                 \
    typedef StcFatp(const long int ptr const ptr)                              \
        StcFatp(long const int ptr const ptr);                                 \
    typedef StcFatp(const long int ptr const ptr)                              \
        StcFatp(long int const ptr const ptr);                                 \
    typedef StcFatp(const signed long ptr const ptr)                           \
        StcFatp(const long ptr const ptr);                                     \
    typedef StcFatp(const long ptr const ptr)                                  \
        StcFatp(long const ptr const ptr);                                     \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr const ptr);                \
    typedef StcFatp(const unsigned long int ptr const ptr)                     \
        StcFatp(unsigned const long int ptr const ptr);                        \
    typedef StcFatp(const unsigned long int ptr const ptr)                     \
        StcFatp(unsigned long const int ptr const ptr);                        \
    typedef StcFatp(const unsigned long int ptr const ptr)                     \
        StcFatp(unsigned long int const ptr const ptr);                        \
    typedef StcFatp(const unsigned long int ptr const ptr)                     \
        StcFatp(const unsigned long ptr const ptr);                            \
    typedef StcFatp(const unsigned long ptr const ptr)                         \
        StcFatp(unsigned const long ptr const ptr);                            \
    typedef StcFatp(const unsigned long ptr const ptr)                         \
        StcFatp(unsigned long const ptr const ptr);                            \
                                                                               \
    stc_fatp_define_for(signed long int ptr ptr const);                        \
    typedef StcFatp(signed long int ptr ptr const)                             \
        StcFatp(signed long ptr ptr const);                                    \
    typedef StcFatp(signed long int ptr ptr const)                             \
        StcFatp(long int ptr ptr const);                                       \
    typedef StcFatp(signed long ptr ptr const) StcFatp(long ptr ptr const);    \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr ptr const);                      \
    typedef StcFatp(unsigned long int ptr ptr const)                           \
        StcFatp(unsigned long ptr ptr const);                                  \
                                                                               \
    stc_fatp_define_for(const signed long int ptr ptr const);                  \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(signed const long int ptr ptr const);                          \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(signed long const int ptr ptr const);                          \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(signed long int const ptr ptr const);                          \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(const signed long ptr ptr const);                              \
    typedef StcFatp(const signed long ptr ptr const)                           \
        StcFatp(signed const long ptr ptr const);                              \
    typedef StcFatp(const signed long ptr ptr const)                           \
        StcFatp(signed long const ptr ptr const);                              \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(const long int ptr ptr const);                                 \
    typedef StcFatp(const long int ptr ptr const)                              \
        StcFatp(long const int ptr ptr const);                                 \
    typedef StcFatp(const long int ptr ptr const)                              \
        StcFatp(long int const ptr ptr const);                                 \
    typedef StcFatp(const signed long ptr ptr const)                           \
        StcFatp(const long ptr ptr const);                                     \
    typedef StcFatp(const long ptr ptr const)                                  \
        StcFatp(long const ptr ptr const);                                     \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr ptr const);                \
    typedef StcFatp(const unsigned long int ptr ptr const)                     \
        StcFatp(unsigned const long int ptr ptr const);                        \
    typedef StcFatp(const unsigned long int ptr ptr const)                     \
        StcFatp(unsigned long const int ptr ptr const);                        \
    typedef StcFatp(const unsigned long int ptr ptr const)                     \
        StcFatp(unsigned long int const ptr ptr const);                        \
    typedef StcFatp(const unsigned long int ptr ptr const)                     \
        StcFatp(const unsigned long ptr ptr const);                            \
    typedef StcFatp(const unsigned long ptr ptr const)                         \
        StcFatp(unsigned const long ptr ptr const);                            \
    typedef StcFatp(const unsigned long ptr ptr const)                         \
        StcFatp(unsigned long const ptr ptr const);                            \
                                                                               \
    stc_fatp_define_for(signed long int ptr const ptr const);                  \
    typedef StcFatp(signed long int ptr const ptr const)                       \
        StcFatp(signed long ptr const ptr const);                              \
    typedef StcFatp(signed long int ptr const ptr const)                       \
        StcFatp(long int ptr const ptr const);                                 \
    typedef StcFatp(signed long ptr const ptr const)                           \
        StcFatp(long ptr const ptr const);                                     \
                                                                               \
    stc_fatp_define_for(unsigned long int ptr const ptr const);                \
    typedef StcFatp(unsigned long int ptr const ptr const)                     \
        StcFatp(unsigned long ptr const ptr const);                            \
                                                                               \
    stc_fatp_define_for(const signed long int ptr const ptr const);            \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(signed const long int ptr const ptr const);                    \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(signed long const int ptr const ptr const);                    \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(signed long int const ptr const ptr const);                    \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(const signed long ptr const ptr const);                        \
    typedef StcFatp(const signed long ptr const ptr const)                     \
        StcFatp(signed const long ptr const ptr const);                        \
    typedef StcFatp(const signed long ptr const ptr const)                     \
        StcFatp(signed long const ptr const ptr const);                        \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(const long int ptr const ptr const);                           \
    typedef StcFatp(const long int ptr const ptr const)                        \
        StcFatp(long const int ptr const ptr const);                           \
    typedef StcFatp(const long int ptr const ptr const)                        \
        StcFatp(long int const ptr const ptr const);                           \
    typedef StcFatp(const signed long ptr const ptr const)                     \
        StcFatp(const long ptr const ptr const);                               \
    typedef StcFatp(const long ptr const ptr const)                            \
        StcFatp(long const ptr const ptr const);                               \
                                                                               \
    stc_fatp_define_for(const unsigned long int ptr const ptr const);          \
    typedef StcFatp(const unsigned long int ptr const ptr const)               \
        StcFatp(unsigned const long int ptr const ptr const);                  \
    typedef StcFatp(const unsigned long int ptr const ptr const)               \
        StcFatp(unsigned long const int ptr const ptr const);                  \
    typedef StcFatp(const unsigned long int ptr const ptr const)               \
        StcFatp(unsigned long int const ptr const ptr const);                  \
    typedef StcFatp(const unsigned long int ptr const ptr const)               \
        StcFatp(const unsigned long ptr const ptr const);                      \
    typedef StcFatp(const unsigned long ptr const ptr const)                   \
        StcFatp(unsigned const long ptr const ptr const);                      \
    typedef StcFatp(const unsigned long ptr const ptr const)                   \
        StcFatp(unsigned long const ptr const ptr const);                      \
                                                                               \
    stc_fatp_define_for(signed long long int);                                 \
    typedef StcFatp(signed long long int) StcFatp(signed long long);           \
    typedef StcFatp(signed long long int) StcFatp(long long int);              \
    typedef StcFatp(signed long long) StcFatp(long long);                      \
                                                                               \
    stc_fatp_define_for(unsigned long long int);                               \
    typedef StcFatp(unsigned long long int) StcFatp(unsigned long long);       \
                                                                               \
    stc_fatp_define_for(const signed long long int);                           \
    typedef StcFatp(const signed long long int)                                \
        StcFatp(signed const long long int);                                   \
    typedef StcFatp(const signed long long int)                                \
        StcFatp(signed long const long int);                                   \
    typedef StcFatp(const signed long long int)                                \
        StcFatp(signed long long const int);                                   \
    typedef StcFatp(const signed long long int)                                \
        StcFatp(signed long long int const);                                   \
    typedef StcFatp(const signed long long int)                                \
        StcFatp(const signed long long);                                       \
    typedef StcFatp(const signed long long) StcFatp(signed const long long);   \
    typedef StcFatp(const signed long long) StcFatp(signed long const long);   \
    typedef StcFatp(const signed long long) StcFatp(signed long long const);   \
    typedef StcFatp(const signed long long int) StcFatp(const long long int);  \
    typedef StcFatp(const long long int) StcFatp(long const long int);         \
    typedef StcFatp(const long long int) StcFatp(long long const int);         \
    typedef StcFatp(const long long int) StcFatp(long long int const);         \
    typedef StcFatp(const signed long long) StcFatp(const long long);          \
    typedef StcFatp(const long long) StcFatp(long const long);                 \
    typedef StcFatp(const long long) StcFatp(long long const);                 \
                                                                               \
    stc_fatp_define_for(const unsigned long long int);                         \
    typedef StcFatp(const unsigned long long int)                              \
        StcFatp(unsigned const long long int);                                 \
    typedef StcFatp(const unsigned long long int)                              \
        StcFatp(unsigned long const long int);                                 \
    typedef StcFatp(const unsigned long long int)                              \
        StcFatp(unsigned long long const int);                                 \
    typedef StcFatp(const unsigned long long int)                              \
        StcFatp(unsigned long long int const);                                 \
    typedef StcFatp(const unsigned long long int)                              \
        StcFatp(const unsigned long long);                                     \
    typedef StcFatp(const unsigned long long)                                  \
        StcFatp(unsigned const long long);                                     \
    typedef StcFatp(const unsigned long long)                                  \
        StcFatp(unsigned long const long);                                     \
    typedef StcFatp(const unsigned long long)                                  \
        StcFatp(unsigned long long const);                                     \
                                                                               \
    stc_fatp_define_for(signed long long int ptr);                             \
    typedef StcFatp(signed long long int ptr) StcFatp(signed long long ptr);   \
    typedef StcFatp(signed long long int ptr) StcFatp(long long int ptr);      \
    typedef StcFatp(signed long long ptr) StcFatp(long long ptr);              \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr);                           \
    typedef StcFatp(unsigned long long int ptr)                                \
        StcFatp(unsigned long long ptr);                                       \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr);                       \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(signed const long long int ptr);                               \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(signed long const long int ptr);                               \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(signed long long const int ptr);                               \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(signed long long int const ptr);                               \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(const signed long long ptr);                                   \
    typedef StcFatp(const signed long long ptr)                                \
        StcFatp(signed const long long ptr);                                   \
    typedef StcFatp(const signed long long ptr)                                \
        StcFatp(signed long const long ptr);                                   \
    typedef StcFatp(const signed long long ptr)                                \
        StcFatp(signed long long const ptr);                                   \
    typedef StcFatp(const signed long long int ptr)                            \
        StcFatp(const long long int ptr);                                      \
    typedef StcFatp(const long long int ptr) StcFatp(long const long int ptr); \
    typedef StcFatp(const long long int ptr) StcFatp(long long const int ptr); \
    typedef StcFatp(const long long int ptr) StcFatp(long long int const ptr); \
    typedef StcFatp(const signed long long ptr) StcFatp(const long long ptr);  \
    typedef StcFatp(const long long ptr) StcFatp(long const long ptr);         \
    typedef StcFatp(const long long ptr) StcFatp(long long const ptr);         \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr);                     \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(unsigned const long long int ptr);                             \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(unsigned long const long int ptr);                             \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(unsigned long long const int ptr);                             \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(unsigned long long int const ptr);                             \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(const unsigned long long ptr);                                 \
    typedef StcFatp(const unsigned long long ptr)                              \
        StcFatp(unsigned const long long ptr);                                 \
    typedef StcFatp(const unsigned long long ptr)                              \
        StcFatp(unsigned long const long ptr);                                 \
    typedef StcFatp(const unsigned long long ptr)                              \
        StcFatp(unsigned long long const ptr);                                 \
                                                                               \
    stc_fatp_define_for(signed long long int ptr const);                       \
    typedef StcFatp(signed long long int ptr const)                            \
        StcFatp(signed long long ptr const);                                   \
    typedef StcFatp(signed long long int ptr const)                            \
        StcFatp(long long int ptr const);                                      \
    typedef StcFatp(signed long long ptr const) StcFatp(long long ptr const);  \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr const);                     \
    typedef StcFatp(unsigned long long int ptr const)                          \
        StcFatp(unsigned long long ptr const);                                 \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr const);                 \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(signed const long long int ptr const);                         \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(signed long const long int ptr const);                         \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(signed long long const int ptr const);                         \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(signed long long int const ptr const);                         \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(const signed long long ptr const);                             \
    typedef StcFatp(const signed long long ptr const)                          \
        StcFatp(signed const long long ptr const);                             \
    typedef StcFatp(const signed long long ptr const)                          \
        StcFatp(signed long const long ptr const);                             \
    typedef StcFatp(const signed long long ptr const)                          \
        StcFatp(signed long long const ptr const);                             \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(const long long int ptr const);                                \
    typedef StcFatp(const long long int ptr const)                             \
        StcFatp(long const long int ptr const);                                \
    typedef StcFatp(const long long int ptr const)                             \
        StcFatp(long long const int ptr const);                                \
    typedef StcFatp(const long long int ptr const)                             \
        StcFatp(long long int const ptr const);                                \
    typedef StcFatp(const signed long long ptr const)                          \
        StcFatp(const long long ptr const);                                    \
    typedef StcFatp(const long long ptr const)                                 \
        StcFatp(long const long ptr const);                                    \
    typedef StcFatp(const long long ptr const)                                 \
        StcFatp(long long const ptr const);                                    \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr const);               \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(unsigned const long long int ptr const);                       \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(unsigned long const long int ptr const);                       \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(unsigned long long const int ptr const);                       \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(unsigned long long int const ptr const);                       \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(const unsigned long long ptr const);                           \
    typedef StcFatp(const unsigned long long ptr const)                        \
        StcFatp(unsigned const long long ptr const);                           \
    typedef StcFatp(const unsigned long long ptr const)                        \
        StcFatp(unsigned long const long ptr const);                           \
    typedef StcFatp(const unsigned long long ptr const)                        \
        StcFatp(unsigned long long const ptr const);                           \
                                                                               \
    stc_fatp_define_for(signed long long int ptr ptr);                         \
    typedef StcFatp(signed long long int ptr ptr)                              \
        StcFatp(signed long long ptr ptr);                                     \
    typedef StcFatp(signed long long int ptr ptr)                              \
        StcFatp(long long int ptr ptr);                                        \
    typedef StcFatp(signed long long ptr ptr) StcFatp(long long ptr ptr);      \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr ptr);                       \
    typedef StcFatp(unsigned long long int ptr ptr)                            \
        StcFatp(unsigned long long ptr ptr);                                   \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr ptr);                   \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(signed const long long int ptr ptr);                           \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(signed long const long int ptr ptr);                           \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(signed long long const int ptr ptr);                           \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(signed long long int const ptr ptr);                           \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(const signed long long ptr ptr);                               \
    typedef StcFatp(const signed long long ptr ptr)                            \
        StcFatp(signed const long long ptr ptr);                               \
    typedef StcFatp(const signed long long ptr ptr)                            \
        StcFatp(signed long const long ptr ptr);                               \
    typedef StcFatp(const signed long long ptr ptr)                            \
        StcFatp(signed long long const ptr ptr);                               \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(const long long int ptr ptr);                                  \
    typedef StcFatp(const long long int ptr ptr)                               \
        StcFatp(long const long int ptr ptr);                                  \
    typedef StcFatp(const long long int ptr ptr)                               \
        StcFatp(long long const int ptr ptr);                                  \
    typedef StcFatp(const long long int ptr ptr)                               \
        StcFatp(long long int const ptr ptr);                                  \
    typedef StcFatp(const signed long long ptr ptr)                            \
        StcFatp(const long long ptr ptr);                                      \
    typedef StcFatp(const long long ptr ptr) StcFatp(long const long ptr ptr); \
    typedef StcFatp(const long long ptr ptr) StcFatp(long long const ptr ptr); \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr ptr);                 \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(unsigned const long long int ptr ptr);                         \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(unsigned long const long int ptr ptr);                         \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(unsigned long long const int ptr ptr);                         \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(unsigned long long int const ptr ptr);                         \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(const unsigned long long ptr ptr);                             \
    typedef StcFatp(const unsigned long long ptr ptr)                          \
        StcFatp(unsigned const long long ptr ptr);                             \
    typedef StcFatp(const unsigned long long ptr ptr)                          \
        StcFatp(unsigned long const long ptr ptr);                             \
    typedef StcFatp(const unsigned long long ptr ptr)                          \
        StcFatp(unsigned long long const ptr ptr);                             \
                                                                               \
    stc_fatp_define_for(signed long long int ptr const ptr);                   \
    typedef StcFatp(signed long long int ptr const ptr)                        \
        StcFatp(signed long long ptr const ptr);                               \
    typedef StcFatp(signed long long int ptr const ptr)                        \
        StcFatp(long long int ptr const ptr);                                  \
    typedef StcFatp(signed long long ptr const ptr)                            \
        StcFatp(long long ptr const ptr);                                      \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr const ptr);                 \
    typedef StcFatp(unsigned long long int ptr const ptr)                      \
        StcFatp(unsigned long long ptr const ptr);                             \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr const ptr);             \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(signed const long long int ptr const ptr);                     \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(signed long const long int ptr const ptr);                     \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(signed long long const int ptr const ptr);                     \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(signed long long int const ptr const ptr);                     \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(const signed long long ptr const ptr);                         \
    typedef StcFatp(const signed long long ptr const ptr)                      \
        StcFatp(signed const long long ptr const ptr);                         \
    typedef StcFatp(const signed long long ptr const ptr)                      \
        StcFatp(signed long const long ptr const ptr);                         \
    typedef StcFatp(const signed long long ptr const ptr)                      \
        StcFatp(signed long long const ptr const ptr);                         \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(const long long int ptr const ptr);                            \
    typedef StcFatp(const long long int ptr const ptr)                         \
        StcFatp(long const long int ptr const ptr);                            \
    typedef StcFatp(const long long int ptr const ptr)                         \
        StcFatp(long long const int ptr const ptr);                            \
    typedef StcFatp(const long long int ptr const ptr)                         \
        StcFatp(long long int const ptr const ptr);                            \
    typedef StcFatp(const signed long long ptr const ptr)                      \
        StcFatp(const long long ptr const ptr);                                \
    typedef StcFatp(const long long ptr const ptr)                             \
        StcFatp(long const long ptr const ptr);                                \
    typedef StcFatp(const long long ptr const ptr)                             \
        StcFatp(long long const ptr const ptr);                                \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr const ptr);           \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(unsigned const long long int ptr const ptr);                   \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(unsigned long const long int ptr const ptr);                   \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(unsigned long long const int ptr const ptr);                   \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(unsigned long long int const ptr const ptr);                   \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(const unsigned long long ptr const ptr);                       \
    typedef StcFatp(const unsigned long long ptr const ptr)                    \
        StcFatp(unsigned const long long ptr const ptr);                       \
    typedef StcFatp(const unsigned long long ptr const ptr)                    \
        StcFatp(unsigned long const long ptr const ptr);                       \
    typedef StcFatp(const unsigned long long ptr const ptr)                    \
        StcFatp(unsigned long long const ptr const ptr);                       \
                                                                               \
    stc_fatp_define_for(signed long long int ptr ptr const);                   \
    typedef StcFatp(signed long long int ptr ptr const)                        \
        StcFatp(signed long long ptr ptr const);                               \
    typedef StcFatp(signed long long int ptr ptr const)                        \
        StcFatp(long long int ptr ptr const);                                  \
    typedef StcFatp(signed long long ptr ptr const)                            \
        StcFatp(long long ptr ptr const);                                      \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr ptr const);                 \
    typedef StcFatp(unsigned long long int ptr ptr const)                      \
        StcFatp(unsigned long long ptr ptr const);                             \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr ptr const);             \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(signed const long long int ptr ptr const);                     \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(signed long const long int ptr ptr const);                     \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(signed long long const int ptr ptr const);                     \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(signed long long int const ptr ptr const);                     \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(const signed long long ptr ptr const);                         \
    typedef StcFatp(const signed long long ptr ptr const)                      \
        StcFatp(signed const long long ptr ptr const);                         \
    typedef StcFatp(const signed long long ptr ptr const)                      \
        StcFatp(signed long const long ptr ptr const);                         \
    typedef StcFatp(const signed long long ptr ptr const)                      \
        StcFatp(signed long long const ptr ptr const);                         \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(const long long int ptr ptr const);                            \
    typedef StcFatp(const long long int ptr ptr const)                         \
        StcFatp(long const long int ptr ptr const);                            \
    typedef StcFatp(const long long int ptr ptr const)                         \
        StcFatp(long long const int ptr ptr const);                            \
    typedef StcFatp(const long long int ptr ptr const)                         \
        StcFatp(long long int const ptr ptr const);                            \
    typedef StcFatp(const signed long long ptr ptr const)                      \
        StcFatp(const long long ptr ptr const);                                \
    typedef StcFatp(const long long ptr ptr const)                             \
        StcFatp(long const long ptr ptr const);                                \
    typedef StcFatp(const long long ptr ptr const)                             \
        StcFatp(long long const ptr ptr const);                                \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr ptr const);           \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(unsigned const long long int ptr ptr const);                   \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(unsigned long const long int ptr ptr const);                   \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(unsigned long long const int ptr ptr const);                   \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(unsigned long long int const ptr ptr const);                   \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(const unsigned long long ptr ptr const);                       \
    typedef StcFatp(const unsigned long long ptr ptr const)                    \
        StcFatp(unsigned const long long ptr ptr const);                       \
    typedef StcFatp(const unsigned long long ptr ptr const)                    \
        StcFatp(unsigned long const long ptr ptr const);                       \
    typedef StcFatp(const unsigned long long ptr ptr const)                    \
        StcFatp(unsigned long long const ptr ptr const);                       \
                                                                               \
    stc_fatp_define_for(signed long long int ptr const ptr const);             \
    typedef StcFatp(signed long long int ptr const ptr const)                  \
        StcFatp(signed long long ptr const ptr const);                         \
    typedef StcFatp(signed long long int ptr const ptr const)                  \
        StcFatp(long long int ptr const ptr const);                            \
    typedef StcFatp(signed long long ptr const ptr const)                      \
        StcFatp(long long ptr const ptr const);                                \
                                                                               \
    stc_fatp_define_for(unsigned long long int ptr const ptr const);           \
    typedef StcFatp(unsigned long long int ptr const ptr const)                \
        StcFatp(unsigned long long ptr const ptr const);                       \
                                                                               \
    stc_fatp_define_for(const signed long long int ptr const ptr const);       \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(signed const long long int ptr const ptr const);               \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(signed long const long int ptr const ptr const);               \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(signed long long const int ptr const ptr const);               \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(signed long long int const ptr const ptr const);               \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(const signed long long ptr const ptr const);                   \
    typedef StcFatp(const signed long long ptr const ptr const)                \
        StcFatp(signed const long long ptr const ptr const);                   \
    typedef StcFatp(const signed long long ptr const ptr const)                \
        StcFatp(signed long const long ptr const ptr const);                   \
    typedef StcFatp(const signed long long ptr const ptr const)                \
        StcFatp(signed long long const ptr const ptr const);                   \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(const long long int ptr const ptr const);                      \
    typedef StcFatp(const long long int ptr const ptr const)                   \
        StcFatp(long const long int ptr const ptr const);                      \
    typedef StcFatp(const long long int ptr const ptr const)                   \
        StcFatp(long long const int ptr const ptr const);                      \
    typedef StcFatp(const long long int ptr const ptr const)                   \
        StcFatp(long long int const ptr const ptr const);                      \
    typedef StcFatp(const signed long long ptr const ptr const)                \
        StcFatp(const long long ptr const ptr const);                          \
    typedef StcFatp(const long long ptr const ptr const)                       \
        StcFatp(long const long ptr const ptr const);                          \
    typedef StcFatp(const long long ptr const ptr const)                       \
        StcFatp(long long const ptr const ptr const);                          \
                                                                               \
    stc_fatp_define_for(const unsigned long long int ptr const ptr const);     \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(unsigned const long long int ptr const ptr const);             \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(unsigned long const long int ptr const ptr const);             \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(unsigned long long const int ptr const ptr const);             \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(unsigned long long int const ptr const ptr const);             \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(const unsigned long long ptr const ptr const);                 \
    typedef StcFatp(const unsigned long long ptr const ptr const)              \
        StcFatp(unsigned const long long ptr const ptr const);                 \
    typedef StcFatp(const unsigned long long ptr const ptr const)              \
        StcFatp(unsigned long const long ptr const ptr const);                 \
    typedef StcFatp(const unsigned long long ptr const ptr const)              \
        StcFatp(unsigned long long const ptr const ptr const);                 \
                                                                               \
    stc_fatp_define_for(float);                                                \
    stc_fatp_define_for(const float);                                          \
    typedef StcFatp(const float) StcFatp(float const);                         \
                                                                               \
    stc_fatp_define_for(float ptr);                                            \
    stc_fatp_define_for(const float ptr);                                      \
    typedef StcFatp(const float ptr) StcFatp(float const ptr);                 \
                                                                               \
    stc_fatp_define_for(float ptr const);                                      \
    stc_fatp_define_for(const float ptr const);                                \
    typedef StcFatp(const float ptr const) StcFatp(float const ptr const);     \
                                                                               \
    stc_fatp_define_for(float ptr ptr);                                        \
    stc_fatp_define_for(const float ptr ptr);                                  \
    typedef StcFatp(const float ptr ptr) StcFatp(float const ptr ptr);         \
                                                                               \
    stc_fatp_define_for(float ptr const ptr);                                  \
    stc_fatp_define_for(const float ptr const ptr);                            \
    typedef StcFatp(const float ptr const ptr)                                 \
        StcFatp(float const ptr const ptr);                                    \
                                                                               \
    stc_fatp_define_for(float ptr ptr const);                                  \
    stc_fatp_define_for(const float ptr ptr const);                            \
    typedef StcFatp(const float ptr ptr const)                                 \
        StcFatp(float const ptr ptr const);                                    \
                                                                               \
    stc_fatp_define_for(float ptr const ptr const);                            \
    stc_fatp_define_for(const float ptr const ptr const);                      \
    typedef StcFatp(const float ptr const ptr const)                           \
        StcFatp(float const ptr const ptr const);                              \
                                                                               \
    stc_fatp_define_for(double);                                               \
    stc_fatp_define_for(const double);                                         \
    typedef StcFatp(const double) StcFatp(double const);                       \
                                                                               \
    stc_fatp_define_for(double ptr);                                           \
    stc_fatp_define_for(const double ptr);                                     \
    typedef StcFatp(const double ptr) StcFatp(double const ptr);               \
                                                                               \
    stc_fatp_define_for(double ptr const);                                     \
    stc_fatp_define_for(const double ptr const);                               \
    typedef StcFatp(const double ptr const) StcFatp(double const ptr const);   \
                                                                               \
    stc_fatp_define_for(double ptr ptr);                                       \
    stc_fatp_define_for(const double ptr ptr);                                 \
    typedef StcFatp(const double ptr ptr) StcFatp(double const ptr ptr);       \
                                                                               \
    stc_fatp_define_for(double ptr const ptr);                                 \
    stc_fatp_define_for(const double ptr const ptr);                           \
    typedef StcFatp(const double ptr const ptr)                                \
        StcFatp(double const ptr const ptr);                                   \
                                                                               \
    stc_fatp_define_for(double ptr ptr const);                                 \
    stc_fatp_define_for(const double ptr ptr const);                           \
    typedef StcFatp(const double ptr ptr const)                                \
        StcFatp(double const ptr ptr const);                                   \
                                                                               \
    stc_fatp_define_for(double ptr const ptr const);                           \
    stc_fatp_define_for(const double ptr const ptr const);                     \
    typedef StcFatp(const double ptr const ptr const)                          \
        StcFatp(double const ptr const ptr const);                             \
                                                                               \
    stc_fatp_define_for(long double);                                          \
    stc_fatp_define_for(const long double);                                    \
    typedef StcFatp(const long double) StcFatp(long const double);             \
    typedef StcFatp(const long double) StcFatp(long double const);             \
                                                                               \
    stc_fatp_define_for(long double ptr);                                      \
    stc_fatp_define_for(const long double ptr);                                \
    typedef StcFatp(const long double ptr) StcFatp(long const double ptr);     \
    typedef StcFatp(const long double ptr) StcFatp(long double const ptr);     \
                                                                               \
    stc_fatp_define_for(long double ptr const);                                \
    stc_fatp_define_for(const long double ptr const);                          \
    typedef StcFatp(const long double ptr const)                               \
        StcFatp(long const double ptr const);                                  \
    typedef StcFatp(const long double ptr const)                               \
        StcFatp(long double const ptr const);                                  \
                                                                               \
    stc_fatp_define_for(long double ptr ptr);                                  \
    stc_fatp_define_for(const long double ptr ptr);                            \
    typedef StcFatp(const long double ptr ptr)                                 \
        StcFatp(long const double ptr ptr);                                    \
    typedef StcFatp(const long double ptr ptr)                                 \
        StcFatp(long double const ptr ptr);                                    \
                                                                               \
    stc_fatp_define_for(long double ptr const ptr);                            \
    stc_fatp_define_for(const long double ptr const ptr);                      \
    typedef StcFatp(const long double ptr const ptr)                           \
        StcFatp(long const double ptr const ptr);                              \
    typedef StcFatp(const long double ptr const ptr)                           \
        StcFatp(long double const ptr const ptr);                              \
                                                                               \
    stc_fatp_define_for(long double ptr ptr const);                            \
    stc_fatp_define_for(const long double ptr ptr const);                      \
    typedef StcFatp(const long double ptr ptr const)                           \
        StcFatp(long const double ptr ptr const);                              \
    typedef StcFatp(const long double ptr ptr const)                           \
        StcFatp(long double const ptr ptr const);                              \
                                                                               \
    stc_fatp_define_for(long double ptr const ptr const);                      \
    stc_fatp_define_for(const long double ptr const ptr const);                \
    typedef StcFatp(const long double ptr const ptr const)                     \
        StcFatp(long const double ptr const ptr const);                        \
    typedef StcFatp(const long double ptr const ptr const)                     \
        StcFatp(long double const ptr const ptr const);                        \
                                                                               \
    stc_fatp_define_for(size_t);                                               \
    stc_fatp_define_for(const size_t);                                         \
    typedef StcFatp(const size_t) StcFatp(size_t const);                       \
                                                                               \
    stc_fatp_define_for(size_t ptr);                                           \
    stc_fatp_define_for(const size_t ptr);                                     \
    typedef StcFatp(const size_t ptr) StcFatp(size_t const ptr);               \
                                                                               \
    stc_fatp_define_for(size_t ptr const);                                     \
    stc_fatp_define_for(const size_t ptr const);                               \
    typedef StcFatp(const size_t ptr const) StcFatp(size_t const ptr const);   \
                                                                               \
    stc_fatp_define_for(size_t ptr ptr);                                       \
    stc_fatp_define_for(const size_t ptr ptr);                                 \
    typedef StcFatp(const size_t ptr ptr) StcFatp(size_t const ptr ptr);       \
                                                                               \
    stc_fatp_define_for(size_t ptr const ptr);                                 \
    stc_fatp_define_for(const size_t ptr const ptr);                           \
    typedef StcFatp(const size_t ptr const ptr)                                \
        StcFatp(size_t const ptr const ptr);                                   \
                                                                               \
    stc_fatp_define_for(size_t ptr ptr const);                                 \
    stc_fatp_define_for(const size_t ptr ptr const);                           \
    typedef StcFatp(const size_t ptr ptr const)                                \
        StcFatp(size_t const ptr ptr const);                                   \
                                                                               \
    stc_fatp_define_for(size_t ptr const ptr const);                           \
    stc_fatp_define_for(const size_t ptr const ptr const);                     \
    typedef StcFatp(const size_t ptr const ptr const)                          \
        StcFatp(size_t const ptr const ptr const)

#define _STC_FATP_DEFINE_FOR_STDC99_TYPES(StcFatp, stc_fatp_define_for)        \
    stc_fatp_define_for(int8_t);                                               \
    stc_fatp_define_for(int16_t);                                              \
    stc_fatp_define_for(int32_t);                                              \
    stc_fatp_define_for(int64_t);                                              \
    stc_fatp_define_for(intmax_t);                                             \
    stc_fatp_define_for(uint8_t);                                              \
    stc_fatp_define_for(uint16_t);                                             \
    stc_fatp_define_for(uint32_t);                                             \
    stc_fatp_define_for(uint64_t);                                             \
    stc_fatp_define_for(uintmax_t);                                            \
                                                                               \
    stc_fatp_define_for(const int8_t);                                         \
    stc_fatp_define_for(const int16_t);                                        \
    stc_fatp_define_for(const int32_t);                                        \
    stc_fatp_define_for(const int64_t);                                        \
    stc_fatp_define_for(const intmax_t);                                       \
    stc_fatp_define_for(const uint8_t);                                        \
    stc_fatp_define_for(const uint16_t);                                       \
    stc_fatp_define_for(const uint32_t);                                       \
    stc_fatp_define_for(const uint64_t);                                       \
    stc_fatp_define_for(const uintmax_t);                                      \
                                                                               \
    typedef StcFatp(const int8_t) StcFatp(int8_t const);                       \
    typedef StcFatp(const int16_t) StcFatp(int16_t const);                     \
    typedef StcFatp(const int32_t) StcFatp(int32_t const);                     \
    typedef StcFatp(const int64_t) StcFatp(int64_t const);                     \
    typedef StcFatp(const intmax_t) StcFatp(intmax_t const);                   \
    typedef StcFatp(const uint8_t) StcFatp(uint8_t const);                     \
    typedef StcFatp(const uint16_t) StcFatp(uint16_t const);                   \
    typedef StcFatp(const uint32_t) StcFatp(uint32_t const);                   \
    typedef StcFatp(const uint64_t) StcFatp(uint64_t const);                   \
    typedef StcFatp(const uintmax_t) StcFatp(uintmax_t const);                 \
                                                                               \
    stc_fatp_define_for(int8_t ptr);                                           \
    stc_fatp_define_for(int16_t ptr);                                          \
    stc_fatp_define_for(int32_t ptr);                                          \
    stc_fatp_define_for(int64_t ptr);                                          \
    stc_fatp_define_for(intmax_t ptr);                                         \
    stc_fatp_define_for(uint8_t ptr);                                          \
    stc_fatp_define_for(uint16_t ptr);                                         \
    stc_fatp_define_for(uint32_t ptr);                                         \
    stc_fatp_define_for(uint64_t ptr);                                         \
    stc_fatp_define_for(uintmax_t ptr);                                        \
                                                                               \
    stc_fatp_define_for(const int8_t ptr);                                     \
    stc_fatp_define_for(const int16_t ptr);                                    \
    stc_fatp_define_for(const int32_t ptr);                                    \
    stc_fatp_define_for(const int64_t ptr);                                    \
    stc_fatp_define_for(const intmax_t ptr);                                   \
    stc_fatp_define_for(const uint8_t ptr);                                    \
    stc_fatp_define_for(const uint16_t ptr);                                   \
    stc_fatp_define_for(const uint32_t ptr);                                   \
    stc_fatp_define_for(const uint64_t ptr);                                   \
    stc_fatp_define_for(const uintmax_t ptr);                                  \
                                                                               \
    typedef StcFatp(const int8_t ptr) StcFatp(int8_t const ptr);               \
    typedef StcFatp(const int16_t ptr) StcFatp(int16_t const ptr);             \
    typedef StcFatp(const int32_t ptr) StcFatp(int32_t const ptr);             \
    typedef StcFatp(const int64_t ptr) StcFatp(int64_t const ptr);             \
    typedef StcFatp(const intmax_t ptr) StcFatp(intmax_t const ptr);           \
    typedef StcFatp(const uint8_t ptr) StcFatp(uint8_t const ptr);             \
    typedef StcFatp(const uint16_t ptr) StcFatp(uint16_t const ptr);           \
    typedef StcFatp(const uint32_t ptr) StcFatp(uint32_t const ptr);           \
    typedef StcFatp(const uint64_t ptr) StcFatp(uint64_t const ptr);           \
    typedef StcFatp(const uintmax_t ptr) StcFatp(uintmax_t const ptr);         \
                                                                               \
    stc_fatp_define_for(int8_t ptr const);                                     \
    stc_fatp_define_for(int16_t ptr const);                                    \
    stc_fatp_define_for(int32_t ptr const);                                    \
    stc_fatp_define_for(int64_t ptr const);                                    \
    stc_fatp_define_for(intmax_t ptr const);                                   \
    stc_fatp_define_for(uint8_t ptr const);                                    \
    stc_fatp_define_for(uint16_t ptr const);                                   \
    stc_fatp_define_for(uint32_t ptr const);                                   \
    stc_fatp_define_for(uint64_t ptr const);                                   \
    stc_fatp_define_for(uintmax_t ptr const);                                  \
                                                                               \
    stc_fatp_define_for(const int8_t ptr const);                               \
    stc_fatp_define_for(const int16_t ptr const);                              \
    stc_fatp_define_for(const int32_t ptr const);                              \
    stc_fatp_define_for(const int64_t ptr const);                              \
    stc_fatp_define_for(const intmax_t ptr const);                             \
    stc_fatp_define_for(const uint8_t ptr const);                              \
    stc_fatp_define_for(const uint16_t ptr const);                             \
    stc_fatp_define_for(const uint32_t ptr const);                             \
    stc_fatp_define_for(const uint64_t ptr const);                             \
    stc_fatp_define_for(const uintmax_t ptr const);                            \
                                                                               \
    typedef StcFatp(const int8_t ptr const) StcFatp(int8_t const ptr const);   \
    typedef StcFatp(const int16_t ptr const) StcFatp(int16_t const ptr const); \
    typedef StcFatp(const int32_t ptr const) StcFatp(int32_t const ptr const); \
    typedef StcFatp(const int64_t ptr const) StcFatp(int64_t const ptr const); \
    typedef StcFatp(const intmax_t ptr const)                                  \
        StcFatp(intmax_t const ptr const);                                     \
    typedef StcFatp(const uint8_t ptr const) StcFatp(uint8_t const ptr const); \
    typedef StcFatp(const uint16_t ptr const)                                  \
        StcFatp(uint16_t const ptr const);                                     \
    typedef StcFatp(const uint32_t ptr const)                                  \
        StcFatp(uint32_t const ptr const);                                     \
    typedef StcFatp(const uint64_t ptr const)                                  \
        StcFatp(uint64_t const ptr const);                                     \
    typedef StcFatp(const uintmax_t ptr const)                                 \
        StcFatp(uintmax_t const ptr const);                                    \
                                                                               \
    stc_fatp_define_for(int8_t ptr ptr);                                       \
    stc_fatp_define_for(int16_t ptr ptr);                                      \
    stc_fatp_define_for(int32_t ptr ptr);                                      \
    stc_fatp_define_for(int64_t ptr ptr);                                      \
    stc_fatp_define_for(intmax_t ptr ptr);                                     \
    stc_fatp_define_for(uint8_t ptr ptr);                                      \
    stc_fatp_define_for(uint16_t ptr ptr);                                     \
    stc_fatp_define_for(uint32_t ptr ptr);                                     \
    stc_fatp_define_for(uint64_t ptr ptr);                                     \
    stc_fatp_define_for(uintmax_t ptr ptr);                                    \
                                                                               \
    stc_fatp_define_for(const int8_t ptr ptr);                                 \
    stc_fatp_define_for(const int16_t ptr ptr);                                \
    stc_fatp_define_for(const int32_t ptr ptr);                                \
    stc_fatp_define_for(const int64_t ptr ptr);                                \
    stc_fatp_define_for(const intmax_t ptr ptr);                               \
    stc_fatp_define_for(const uint8_t ptr ptr);                                \
    stc_fatp_define_for(const uint16_t ptr ptr);                               \
    stc_fatp_define_for(const uint32_t ptr ptr);                               \
    stc_fatp_define_for(const uint64_t ptr ptr);                               \
    stc_fatp_define_for(const uintmax_t ptr ptr);                              \
                                                                               \
    typedef StcFatp(const int8_t ptr ptr) StcFatp(int8_t const ptr ptr);       \
    typedef StcFatp(const int16_t ptr ptr) StcFatp(int16_t const ptr ptr);     \
    typedef StcFatp(const int32_t ptr ptr) StcFatp(int32_t const ptr ptr);     \
    typedef StcFatp(const int64_t ptr ptr) StcFatp(int64_t const ptr ptr);     \
    typedef StcFatp(const intmax_t ptr ptr) StcFatp(intmax_t const ptr ptr);   \
    typedef StcFatp(const uint8_t ptr ptr) StcFatp(uint8_t const ptr ptr);     \
    typedef StcFatp(const uint16_t ptr ptr) StcFatp(uint16_t const ptr ptr);   \
    typedef StcFatp(const uint32_t ptr ptr) StcFatp(uint32_t const ptr ptr);   \
    typedef StcFatp(const uint64_t ptr ptr) StcFatp(uint64_t const ptr ptr);   \
    typedef StcFatp(const uintmax_t ptr ptr) StcFatp(uintmax_t const ptr ptr); \
                                                                               \
    stc_fatp_define_for(int8_t ptr const ptr);                                 \
    stc_fatp_define_for(int16_t ptr const ptr);                                \
    stc_fatp_define_for(int32_t ptr const ptr);                                \
    stc_fatp_define_for(int64_t ptr const ptr);                                \
    stc_fatp_define_for(intmax_t ptr const ptr);                               \
    stc_fatp_define_for(uint8_t ptr const ptr);                                \
    stc_fatp_define_for(uint16_t ptr const ptr);                               \
    stc_fatp_define_for(uint32_t ptr const ptr);                               \
    stc_fatp_define_for(uint64_t ptr const ptr);                               \
    stc_fatp_define_for(uintmax_t ptr const ptr);                              \
                                                                               \
    stc_fatp_define_for(const int8_t ptr const ptr);                           \
    stc_fatp_define_for(const int16_t ptr const ptr);                          \
    stc_fatp_define_for(const int32_t ptr const ptr);                          \
    stc_fatp_define_for(const int64_t ptr const ptr);                          \
    stc_fatp_define_for(const intmax_t ptr const ptr);                         \
    stc_fatp_define_for(const uint8_t ptr const ptr);                          \
    stc_fatp_define_for(const uint16_t ptr const ptr);                         \
    stc_fatp_define_for(const uint32_t ptr const ptr);                         \
    stc_fatp_define_for(const uint64_t ptr const ptr);                         \
    stc_fatp_define_for(const uintmax_t ptr const ptr);                        \
                                                                               \
    typedef StcFatp(const int8_t ptr const ptr)                                \
        StcFatp(int8_t const ptr const ptr);                                   \
    typedef StcFatp(const int16_t ptr const ptr)                               \
        StcFatp(int16_t const ptr const ptr);                                  \
    typedef StcFatp(const int32_t ptr const ptr)                               \
        StcFatp(int32_t const ptr const ptr);                                  \
    typedef StcFatp(const int64_t ptr const ptr)                               \
        StcFatp(int64_t const ptr const ptr);                                  \
    typedef StcFatp(const intmax_t ptr const ptr)                              \
        StcFatp(intmax_t const ptr const ptr);                                 \
    typedef StcFatp(const uint8_t ptr const ptr)                               \
        StcFatp(uint8_t const ptr const ptr);                                  \
    typedef StcFatp(const uint16_t ptr const ptr)                              \
        StcFatp(uint16_t const ptr const ptr);                                 \
    typedef StcFatp(const uint32_t ptr const ptr)                              \
        StcFatp(uint32_t const ptr const ptr);                                 \
    typedef StcFatp(const uint64_t ptr const ptr)                              \
        StcFatp(uint64_t const ptr const ptr);                                 \
    typedef StcFatp(const uintmax_t ptr const ptr)                             \
        StcFatp(uintmax_t const ptr const ptr);                                \
                                                                               \
    stc_fatp_define_for(int8_t ptr ptr const);                                 \
    stc_fatp_define_for(int16_t ptr ptr const);                                \
    stc_fatp_define_for(int32_t ptr ptr const);                                \
    stc_fatp_define_for(int64_t ptr ptr const);                                \
    stc_fatp_define_for(intmax_t ptr ptr const);                               \
    stc_fatp_define_for(uint8_t ptr ptr const);                                \
    stc_fatp_define_for(uint16_t ptr ptr const);                               \
    stc_fatp_define_for(uint32_t ptr ptr const);                               \
    stc_fatp_define_for(uint64_t ptr ptr const);                               \
    stc_fatp_define_for(uintmax_t ptr ptr const);                              \
                                                                               \
    stc_fatp_define_for(const int8_t ptr ptr const);                           \
    stc_fatp_define_for(const int16_t ptr ptr const);                          \
    stc_fatp_define_for(const int32_t ptr ptr const);                          \
    stc_fatp_define_for(const int64_t ptr ptr const);                          \
    stc_fatp_define_for(const intmax_t ptr ptr const);                         \
    stc_fatp_define_for(const uint8_t ptr ptr const);                          \
    stc_fatp_define_for(const uint16_t ptr ptr const);                         \
    stc_fatp_define_for(const uint32_t ptr ptr const);                         \
    stc_fatp_define_for(const uint64_t ptr ptr const);                         \
    stc_fatp_define_for(const uintmax_t ptr ptr const);                        \
                                                                               \
    typedef StcFatp(const int8_t ptr ptr const)                                \
        StcFatp(int8_t const ptr ptr const);                                   \
    typedef StcFatp(const int16_t ptr ptr const)                               \
        StcFatp(int16_t const ptr ptr const);                                  \
    typedef StcFatp(const int32_t ptr ptr const)                               \
        StcFatp(int32_t const ptr ptr const);                                  \
    typedef StcFatp(const int64_t ptr ptr const)                               \
        StcFatp(int64_t const ptr ptr const);                                  \
    typedef StcFatp(const intmax_t ptr ptr const)                              \
        StcFatp(intmax_t const ptr ptr const);                                 \
    typedef StcFatp(const uint8_t ptr ptr const)                               \
        StcFatp(uint8_t const ptr ptr const);                                  \
    typedef StcFatp(const uint16_t ptr ptr const)                              \
        StcFatp(uint16_t const ptr ptr const);                                 \
    typedef StcFatp(const uint32_t ptr ptr const)                              \
        StcFatp(uint32_t const ptr ptr const);                                 \
    typedef StcFatp(const uint64_t ptr ptr const)                              \
        StcFatp(uint64_t const ptr ptr const);                                 \
    typedef StcFatp(const uintmax_t ptr ptr const)                             \
        StcFatp(uintmax_t const ptr ptr const);                                \
                                                                               \
    stc_fatp_define_for(int8_t ptr const ptr const);                           \
    stc_fatp_define_for(int16_t ptr const ptr const);                          \
    stc_fatp_define_for(int32_t ptr const ptr const);                          \
    stc_fatp_define_for(int64_t ptr const ptr const);                          \
    stc_fatp_define_for(intmax_t ptr const ptr const);                         \
    stc_fatp_define_for(uint8_t ptr const ptr const);                          \
    stc_fatp_define_for(uint16_t ptr const ptr const);                         \
    stc_fatp_define_for(uint32_t ptr const ptr const);                         \
    stc_fatp_define_for(uint64_t ptr const ptr const);                         \
    stc_fatp_define_for(uintmax_t ptr const ptr const);                        \
                                                                               \
    stc_fatp_define_for(const int8_t ptr const ptr const);                     \
    stc_fatp_define_for(const int16_t ptr const ptr const);                    \
    stc_fatp_define_for(const int32_t ptr const ptr const);                    \
    stc_fatp_define_for(const int64_t ptr const ptr const);                    \
    stc_fatp_define_for(const intmax_t ptr const ptr const);                   \
    stc_fatp_define_for(const uint8_t ptr const ptr const);                    \
    stc_fatp_define_for(const uint16_t ptr const ptr const);                   \
    stc_fatp_define_for(const uint32_t ptr const ptr const);                   \
    stc_fatp_define_for(const uint64_t ptr const ptr const);                   \
    stc_fatp_define_for(const uintmax_t ptr const ptr const);                  \
                                                                               \
    typedef StcFatp(const int8_t ptr const ptr const)                          \
        StcFatp(int8_t const ptr const ptr const);                             \
    typedef StcFatp(const int16_t ptr const ptr const)                         \
        StcFatp(int16_t const ptr const ptr const);                            \
    typedef StcFatp(const int32_t ptr const ptr const)                         \
        StcFatp(int32_t const ptr const ptr const);                            \
    typedef StcFatp(const int64_t ptr const ptr const)                         \
        StcFatp(int64_t const ptr const ptr const);                            \
    typedef StcFatp(const intmax_t ptr const ptr const)                        \
        StcFatp(intmax_t const ptr const ptr const);                           \
    typedef StcFatp(const uint8_t ptr const ptr const)                         \
        StcFatp(uint8_t const ptr const ptr const);                            \
    typedef StcFatp(const uint16_t ptr const ptr const)                        \
        StcFatp(uint16_t const ptr const ptr const);                           \
    typedef StcFatp(const uint32_t ptr const ptr const)                        \
        StcFatp(uint32_t const ptr const ptr const);                           \
    typedef StcFatp(const uint64_t ptr const ptr const)                        \
        StcFatp(uint64_t const ptr const ptr const);                           \
    typedef StcFatp(const uintmax_t ptr const ptr const)                       \
        StcFatp(uintmax_t const ptr const ptr const)

#define _STC_FATP_ALIAS_TYPES_COMMON_FROM_STDC99(StcFatp)                      \
    typedef StcFatp(int8_t) StcFatp(i8);                                       \
    typedef StcFatp(int16_t) StcFatp(i16);                                     \
    typedef StcFatp(int32_t) StcFatp(i32);                                     \
    typedef StcFatp(int64_t) StcFatp(i64);                                     \
    typedef StcFatp(intmax_t) StcFatp(isize);                                  \
    typedef StcFatp(uint8_t) StcFatp(u8);                                      \
    typedef StcFatp(uint16_t) StcFatp(u16);                                    \
    typedef StcFatp(uint32_t) StcFatp(u32);                                    \
    typedef StcFatp(uint64_t) StcFatp(u64);                                    \
    typedef StcFatp(uintmax_t) StcFatp(usize);                                 \
                                                                               \
    typedef StcFatp(const int8_t) StcFatp(const i8);                           \
    typedef StcFatp(const int16_t) StcFatp(const i16);                         \
    typedef StcFatp(const int32_t) StcFatp(const i32);                         \
    typedef StcFatp(const int64_t) StcFatp(const i64);                         \
    typedef StcFatp(const intmax_t) StcFatp(const isize);                      \
    typedef StcFatp(const uint8_t) StcFatp(const u8);                          \
    typedef StcFatp(const uint16_t) StcFatp(const u16);                        \
    typedef StcFatp(const uint32_t) StcFatp(const u32);                        \
    typedef StcFatp(const uint64_t) StcFatp(const u64);                        \
    typedef StcFatp(const uintmax_t) StcFatp(const usize);                     \
                                                                               \
    typedef StcFatp(int8_t const) StcFatp(i8 const);                           \
    typedef StcFatp(int16_t const) StcFatp(i16 const);                         \
    typedef StcFatp(int32_t const) StcFatp(i32 const);                         \
    typedef StcFatp(int64_t const) StcFatp(i64 const);                         \
    typedef StcFatp(intmax_t const) StcFatp(isize const);                      \
    typedef StcFatp(uint8_t const) StcFatp(u8 const);                          \
    typedef StcFatp(uint16_t const) StcFatp(u16 const);                        \
    typedef StcFatp(uint32_t const) StcFatp(u32 const);                        \
    typedef StcFatp(uint64_t const) StcFatp(u64 const);                        \
    typedef StcFatp(uintmax_t const) StcFatp(usize const);                     \
                                                                               \
    typedef StcFatp(int8_t ptr) StcFatp(i8 ptr);                               \
    typedef StcFatp(int16_t ptr) StcFatp(i16 ptr);                             \
    typedef StcFatp(int32_t ptr) StcFatp(i32 ptr);                             \
    typedef StcFatp(int64_t ptr) StcFatp(i64 ptr);                             \
    typedef StcFatp(intmax_t ptr) StcFatp(isize ptr);                          \
    typedef StcFatp(uint8_t ptr) StcFatp(u8 ptr);                              \
    typedef StcFatp(uint16_t ptr) StcFatp(u16 ptr);                            \
    typedef StcFatp(uint32_t ptr) StcFatp(u32 ptr);                            \
    typedef StcFatp(uint64_t ptr) StcFatp(u64 ptr);                            \
    typedef StcFatp(uintmax_t ptr) StcFatp(usize ptr);                         \
                                                                               \
    typedef StcFatp(const int8_t ptr) StcFatp(const i8 ptr);                   \
    typedef StcFatp(const int16_t ptr) StcFatp(const i16 ptr);                 \
    typedef StcFatp(const int32_t ptr) StcFatp(const i32 ptr);                 \
    typedef StcFatp(const int64_t ptr) StcFatp(const i64 ptr);                 \
    typedef StcFatp(const intmax_t ptr) StcFatp(const isize ptr);              \
    typedef StcFatp(const uint8_t ptr) StcFatp(const u8 ptr);                  \
    typedef StcFatp(const uint16_t ptr) StcFatp(const u16 ptr);                \
    typedef StcFatp(const uint32_t ptr) StcFatp(const u32 ptr);                \
    typedef StcFatp(const uint64_t ptr) StcFatp(const u64 ptr);                \
    typedef StcFatp(const uintmax_t ptr) StcFatp(const usize ptr);             \
                                                                               \
    typedef StcFatp(int8_t const ptr) StcFatp(i8 const ptr);                   \
    typedef StcFatp(int16_t const ptr) StcFatp(i16 const ptr);                 \
    typedef StcFatp(int32_t const ptr) StcFatp(i32 const ptr);                 \
    typedef StcFatp(int64_t const ptr) StcFatp(i64 const ptr);                 \
    typedef StcFatp(intmax_t const ptr) StcFatp(isize const ptr);              \
    typedef StcFatp(uint8_t const ptr) StcFatp(u8 const ptr);                  \
    typedef StcFatp(uint16_t const ptr) StcFatp(u16 const ptr);                \
    typedef StcFatp(uint32_t const ptr) StcFatp(u32 const ptr);                \
    typedef StcFatp(uint64_t const ptr) StcFatp(u64 const ptr);                \
    typedef StcFatp(uintmax_t const ptr) StcFatp(usize const ptr);             \
                                                                               \
    typedef StcFatp(int8_t ptr const) StcFatp(i8 ptr const);                   \
    typedef StcFatp(int16_t ptr const) StcFatp(i16 ptr const);                 \
    typedef StcFatp(int32_t ptr const) StcFatp(i32 ptr const);                 \
    typedef StcFatp(int64_t ptr const) StcFatp(i64 ptr const);                 \
    typedef StcFatp(intmax_t ptr const) StcFatp(isize ptr const);              \
    typedef StcFatp(uint8_t ptr const) StcFatp(u8 ptr const);                  \
    typedef StcFatp(uint16_t ptr const) StcFatp(u16 ptr const);                \
    typedef StcFatp(uint32_t ptr const) StcFatp(u32 ptr const);                \
    typedef StcFatp(uint64_t ptr const) StcFatp(u64 ptr const);                \
    typedef StcFatp(uintmax_t ptr const) StcFatp(usize ptr const);             \
                                                                               \
    typedef StcFatp(const int8_t ptr const) StcFatp(const i8 ptr const);       \
    typedef StcFatp(const int16_t ptr const) StcFatp(const i16 ptr const);     \
    typedef StcFatp(const int32_t ptr const) StcFatp(const i32 ptr const);     \
    typedef StcFatp(const int64_t ptr const) StcFatp(const i64 ptr const);     \
    typedef StcFatp(const intmax_t ptr const) StcFatp(const isize ptr const);  \
    typedef StcFatp(const uint8_t ptr const) StcFatp(const u8 ptr const);      \
    typedef StcFatp(const uint16_t ptr const) StcFatp(const u16 ptr const);    \
    typedef StcFatp(const uint32_t ptr const) StcFatp(const u32 ptr const);    \
    typedef StcFatp(const uint64_t ptr const) StcFatp(const u64 ptr const);    \
    typedef StcFatp(const uintmax_t ptr const) StcFatp(const usize ptr const); \
                                                                               \
    typedef StcFatp(int8_t const ptr const) StcFatp(i8 const ptr const);       \
    typedef StcFatp(int16_t const ptr const) StcFatp(i16 const ptr const);     \
    typedef StcFatp(int32_t const ptr const) StcFatp(i32 const ptr const);     \
    typedef StcFatp(int64_t const ptr const) StcFatp(i64 const ptr const);     \
    typedef StcFatp(intmax_t const ptr const) StcFatp(isize const ptr const);  \
    typedef StcFatp(uint8_t const ptr const) StcFatp(u8 const ptr const);      \
    typedef StcFatp(uint16_t const ptr const) StcFatp(u16 const ptr const);    \
    typedef StcFatp(uint32_t const ptr const) StcFatp(u32 const ptr const);    \
    typedef StcFatp(uint64_t const ptr const) StcFatp(u64 const ptr const);    \
    typedef StcFatp(uintmax_t const ptr const) StcFatp(usize const ptr const); \
                                                                               \
    typedef StcFatp(int8_t ptr ptr) StcFatp(i8 ptr ptr);                       \
    typedef StcFatp(int16_t ptr ptr) StcFatp(i16 ptr ptr);                     \
    typedef StcFatp(int32_t ptr ptr) StcFatp(i32 ptr ptr);                     \
    typedef StcFatp(int64_t ptr ptr) StcFatp(i64 ptr ptr);                     \
    typedef StcFatp(intmax_t ptr ptr) StcFatp(isize ptr ptr);                  \
    typedef StcFatp(uint8_t ptr ptr) StcFatp(u8 ptr ptr);                      \
    typedef StcFatp(uint16_t ptr ptr) StcFatp(u16 ptr ptr);                    \
    typedef StcFatp(uint32_t ptr ptr) StcFatp(u32 ptr ptr);                    \
    typedef StcFatp(uint64_t ptr ptr) StcFatp(u64 ptr ptr);                    \
    typedef StcFatp(uintmax_t ptr ptr) StcFatp(usize ptr ptr);                 \
                                                                               \
    typedef StcFatp(const int8_t ptr ptr) StcFatp(const i8 ptr ptr);           \
    typedef StcFatp(const int16_t ptr ptr) StcFatp(const i16 ptr ptr);         \
    typedef StcFatp(const int32_t ptr ptr) StcFatp(const i32 ptr ptr);         \
    typedef StcFatp(const int64_t ptr ptr) StcFatp(const i64 ptr ptr);         \
    typedef StcFatp(const intmax_t ptr ptr) StcFatp(const isize ptr ptr);      \
    typedef StcFatp(const uint8_t ptr ptr) StcFatp(const u8 ptr ptr);          \
    typedef StcFatp(const uint16_t ptr ptr) StcFatp(const u16 ptr ptr);        \
    typedef StcFatp(const uint32_t ptr ptr) StcFatp(const u32 ptr ptr);        \
    typedef StcFatp(const uint64_t ptr ptr) StcFatp(const u64 ptr ptr);        \
    typedef StcFatp(const uintmax_t ptr ptr) StcFatp(const usize ptr ptr);     \
                                                                               \
    typedef StcFatp(int8_t const ptr ptr) StcFatp(i8 const ptr ptr);           \
    typedef StcFatp(int16_t const ptr ptr) StcFatp(i16 const ptr ptr);         \
    typedef StcFatp(int32_t const ptr ptr) StcFatp(i32 const ptr ptr);         \
    typedef StcFatp(int64_t const ptr ptr) StcFatp(i64 const ptr ptr);         \
    typedef StcFatp(intmax_t const ptr ptr) StcFatp(isize const ptr ptr);      \
    typedef StcFatp(uint8_t const ptr ptr) StcFatp(u8 const ptr ptr);          \
    typedef StcFatp(uint16_t const ptr ptr) StcFatp(u16 const ptr ptr);        \
    typedef StcFatp(uint32_t const ptr ptr) StcFatp(u32 const ptr ptr);        \
    typedef StcFatp(uint64_t const ptr ptr) StcFatp(u64 const ptr ptr);        \
    typedef StcFatp(uintmax_t const ptr ptr) StcFatp(usize const ptr ptr);     \
                                                                               \
    typedef StcFatp(int8_t ptr const ptr) StcFatp(i8 ptr const ptr);           \
    typedef StcFatp(int16_t ptr const ptr) StcFatp(i16 ptr const ptr);         \
    typedef StcFatp(int32_t ptr const ptr) StcFatp(i32 ptr const ptr);         \
    typedef StcFatp(int64_t ptr const ptr) StcFatp(i64 ptr const ptr);         \
    typedef StcFatp(intmax_t ptr const ptr) StcFatp(isize ptr const ptr);      \
    typedef StcFatp(uint8_t ptr const ptr) StcFatp(u8 ptr const ptr);          \
    typedef StcFatp(uint16_t ptr const ptr) StcFatp(u16 ptr const ptr);        \
    typedef StcFatp(uint32_t ptr const ptr) StcFatp(u32 ptr const ptr);        \
    typedef StcFatp(uint64_t ptr const ptr) StcFatp(u64 ptr const ptr);        \
    typedef StcFatp(uintmax_t ptr const ptr) StcFatp(usize ptr const ptr);     \
                                                                               \
    typedef StcFatp(const int8_t ptr const ptr)                                \
        StcFatp(const i8 ptr const ptr);                                       \
    typedef StcFatp(const int16_t ptr const ptr)                               \
        StcFatp(const i16 ptr const ptr);                                      \
    typedef StcFatp(const int32_t ptr const ptr)                               \
        StcFatp(const i32 ptr const ptr);                                      \
    typedef StcFatp(const int64_t ptr const ptr)                               \
        StcFatp(const i64 ptr const ptr);                                      \
    typedef StcFatp(const intmax_t ptr const ptr)                              \
        StcFatp(const isize ptr const ptr);                                    \
    typedef StcFatp(const uint8_t ptr const ptr)                               \
        StcFatp(const u8 ptr const ptr);                                       \
    typedef StcFatp(const uint16_t ptr const ptr)                              \
        StcFatp(const u16 ptr const ptr);                                      \
    typedef StcFatp(const uint32_t ptr const ptr)                              \
        StcFatp(const u32 ptr const ptr);                                      \
    typedef StcFatp(const uint64_t ptr const ptr)                              \
        StcFatp(const u64 ptr const ptr);                                      \
    typedef StcFatp(const uintmax_t ptr const ptr)                             \
        StcFatp(const usize ptr const ptr);                                    \
                                                                               \
    typedef StcFatp(int8_t const ptr const ptr)                                \
        StcFatp(i8 const ptr const ptr);                                       \
    typedef StcFatp(int16_t const ptr const ptr)                               \
        StcFatp(i16 const ptr const ptr);                                      \
    typedef StcFatp(int32_t const ptr const ptr)                               \
        StcFatp(i32 const ptr const ptr);                                      \
    typedef StcFatp(int64_t const ptr const ptr)                               \
        StcFatp(i64 const ptr const ptr);                                      \
    typedef StcFatp(intmax_t const ptr const ptr)                              \
        StcFatp(isize const ptr const ptr);                                    \
    typedef StcFatp(uint8_t const ptr const ptr)                               \
        StcFatp(u8 const ptr const ptr);                                       \
    typedef StcFatp(uint16_t const ptr const ptr)                              \
        StcFatp(u16 const ptr const ptr);                                      \
    typedef StcFatp(uint32_t const ptr const ptr)                              \
        StcFatp(u32 const ptr const ptr);                                      \
    typedef StcFatp(uint64_t const ptr const ptr)                              \
        StcFatp(u64 const ptr const ptr);                                      \
    typedef StcFatp(uintmax_t const ptr const ptr)                             \
        StcFatp(usize const ptr const ptr);                                    \
                                                                               \
    typedef StcFatp(int8_t ptr ptr const) StcFatp(i8 ptr ptr const);           \
    typedef StcFatp(int16_t ptr ptr const) StcFatp(i16 ptr ptr const);         \
    typedef StcFatp(int32_t ptr ptr const) StcFatp(i32 ptr ptr const);         \
    typedef StcFatp(int64_t ptr ptr const) StcFatp(i64 ptr ptr const);         \
    typedef StcFatp(intmax_t ptr ptr const) StcFatp(isize ptr ptr const);      \
    typedef StcFatp(uint8_t ptr ptr const) StcFatp(u8 ptr ptr const);          \
    typedef StcFatp(uint16_t ptr ptr const) StcFatp(u16 ptr ptr const);        \
    typedef StcFatp(uint32_t ptr ptr const) StcFatp(u32 ptr ptr const);        \
    typedef StcFatp(uint64_t ptr ptr const) StcFatp(u64 ptr ptr const);        \
    typedef StcFatp(uintmax_t ptr ptr const) StcFatp(usize ptr ptr const);     \
                                                                               \
    typedef StcFatp(const int8_t ptr ptr const)                                \
        StcFatp(const i8 ptr ptr const);                                       \
    typedef StcFatp(const int16_t ptr ptr const)                               \
        StcFatp(const i16 ptr ptr const);                                      \
    typedef StcFatp(const int32_t ptr ptr const)                               \
        StcFatp(const i32 ptr ptr const);                                      \
    typedef StcFatp(const int64_t ptr ptr const)                               \
        StcFatp(const i64 ptr ptr const);                                      \
    typedef StcFatp(const intmax_t ptr ptr const)                              \
        StcFatp(const isize ptr ptr const);                                    \
    typedef StcFatp(const uint8_t ptr ptr const)                               \
        StcFatp(const u8 ptr ptr const);                                       \
    typedef StcFatp(const uint16_t ptr ptr const)                              \
        StcFatp(const u16 ptr ptr const);                                      \
    typedef StcFatp(const uint32_t ptr ptr const)                              \
        StcFatp(const u32 ptr ptr const);                                      \
    typedef StcFatp(const uint64_t ptr ptr const)                              \
        StcFatp(const u64 ptr ptr const);                                      \
    typedef StcFatp(const uintmax_t ptr ptr const)                             \
        StcFatp(const usize ptr ptr const);                                    \
                                                                               \
    typedef StcFatp(int8_t const ptr ptr const)                                \
        StcFatp(i8 const ptr ptr const);                                       \
    typedef StcFatp(int16_t const ptr ptr const)                               \
        StcFatp(i16 const ptr ptr const);                                      \
    typedef StcFatp(int32_t const ptr ptr const)                               \
        StcFatp(i32 const ptr ptr const);                                      \
    typedef StcFatp(int64_t const ptr ptr const)                               \
        StcFatp(i64 const ptr ptr const);                                      \
    typedef StcFatp(intmax_t const ptr ptr const)                              \
        StcFatp(isize const ptr ptr const);                                    \
    typedef StcFatp(uint8_t const ptr ptr const)                               \
        StcFatp(u8 const ptr ptr const);                                       \
    typedef StcFatp(uint16_t const ptr ptr const)                              \
        StcFatp(u16 const ptr ptr const);                                      \
    typedef StcFatp(uint32_t const ptr ptr const)                              \
        StcFatp(u32 const ptr ptr const);                                      \
    typedef StcFatp(uint64_t const ptr ptr const)                              \
        StcFatp(u64 const ptr ptr const);                                      \
    typedef StcFatp(uintmax_t const ptr ptr const)                             \
        StcFatp(usize const ptr ptr const);                                    \
                                                                               \
    typedef StcFatp(int8_t ptr const ptr const)                                \
        StcFatp(i8 ptr const ptr const);                                       \
    typedef StcFatp(int16_t ptr const ptr const)                               \
        StcFatp(i16 ptr const ptr const);                                      \
    typedef StcFatp(int32_t ptr const ptr const)                               \
        StcFatp(i32 ptr const ptr const);                                      \
    typedef StcFatp(int64_t ptr const ptr const)                               \
        StcFatp(i64 ptr const ptr const);                                      \
    typedef StcFatp(intmax_t ptr const ptr const)                              \
        StcFatp(isize ptr const ptr const);                                    \
    typedef StcFatp(uint8_t ptr const ptr const)                               \
        StcFatp(u8 ptr const ptr const);                                       \
    typedef StcFatp(uint16_t ptr const ptr const)                              \
        StcFatp(u16 ptr const ptr const);                                      \
    typedef StcFatp(uint32_t ptr const ptr const)                              \
        StcFatp(u32 ptr const ptr const);                                      \
    typedef StcFatp(uint64_t ptr const ptr const)                              \
        StcFatp(u64 ptr const ptr const);                                      \
    typedef StcFatp(uintmax_t ptr const ptr const)                             \
        StcFatp(usize ptr const ptr const);                                    \
                                                                               \
    typedef StcFatp(const int8_t ptr const ptr const)                          \
        StcFatp(const i8 ptr const ptr const);                                 \
    typedef StcFatp(const int16_t ptr const ptr const)                         \
        StcFatp(const i16 ptr const ptr const);                                \
    typedef StcFatp(const int32_t ptr const ptr const)                         \
        StcFatp(const i32 ptr const ptr const);                                \
    typedef StcFatp(const int64_t ptr const ptr const)                         \
        StcFatp(const i64 ptr const ptr const);                                \
    typedef StcFatp(const intmax_t ptr const ptr const)                        \
        StcFatp(const isize ptr const ptr const);                              \
    typedef StcFatp(const uint8_t ptr const ptr const)                         \
        StcFatp(const u8 ptr const ptr const);                                 \
    typedef StcFatp(const uint16_t ptr const ptr const)                        \
        StcFatp(const u16 ptr const ptr const);                                \
    typedef StcFatp(const uint32_t ptr const ptr const)                        \
        StcFatp(const u32 ptr const ptr const);                                \
    typedef StcFatp(const uint64_t ptr const ptr const)                        \
        StcFatp(const u64 ptr const ptr const);                                \
    typedef StcFatp(const uintmax_t ptr const ptr const)                       \
        StcFatp(const usize ptr const ptr const);                              \
                                                                               \
    typedef StcFatp(int8_t const ptr const ptr const)                          \
        StcFatp(i8 const ptr const ptr const);                                 \
    typedef StcFatp(int16_t const ptr const ptr const)                         \
        StcFatp(i16 const ptr const ptr const);                                \
    typedef StcFatp(int32_t const ptr const ptr const)                         \
        StcFatp(i32 const ptr const ptr const);                                \
    typedef StcFatp(int64_t const ptr const ptr const)                         \
        StcFatp(i64 const ptr const ptr const);                                \
    typedef StcFatp(intmax_t const ptr const ptr const)                        \
        StcFatp(isize const ptr const ptr const);                              \
    typedef StcFatp(uint8_t const ptr const ptr const)                         \
        StcFatp(u8 const ptr const ptr const);                                 \
    typedef StcFatp(uint16_t const ptr const ptr const)                        \
        StcFatp(u16 const ptr const ptr const);                                \
    typedef StcFatp(uint32_t const ptr const ptr const)                        \
        StcFatp(u32 const ptr const ptr const);                                \
    typedef StcFatp(uint64_t const ptr const ptr const)                        \
        StcFatp(u64 const ptr const ptr const);                                \
    typedef StcFatp(uintmax_t const ptr const ptr const)                       \
        StcFatp(usize const ptr const ptr const)

#define STC_FATP_ALIAS_TYPES_COMMON_FROM_BUILTIN(StcFatp)                      \
    typedef StcFatp(signed char) StcFatp(i8);                                  \
    typedef StcFatp(signed short int) StcFatp(i16);                            \
    typedef StcFatp(signed int) StcFatp(i32);                                  \
    typedef StcFatp(signed long int) StcFatp(i64);                             \
    typedef StcFatp(signed long long int) StcFatp(isize);                      \
    typedef StcFatp(unsigned char) StcFatp(u8);                                \
    typedef StcFatp(unsigned short int) StcFatp(u16);                          \
    typedef StcFatp(unsigned int) StcFatp(u32);                                \
    typedef StcFatp(unsigned long int) StcFatp(u64);                           \
    typedef StcFatp(unsigned long long int) StcFatp(usize);                    \
                                                                               \
    typedef StcFatp(const signed char) StcFatp(const i8);                      \
    typedef StcFatp(const signed short int) StcFatp(const i16);                \
    typedef StcFatp(const signed int) StcFatp(const i32);                      \
    typedef StcFatp(const signed long int) StcFatp(const i64);                 \
    typedef StcFatp(const signed long long int) StcFatp(const isize);          \
    typedef StcFatp(const unsigned char) StcFatp(const u8);                    \
    typedef StcFatp(const unsigned short int) StcFatp(const u16);              \
    typedef StcFatp(const unsigned int) StcFatp(const u32);                    \
    typedef StcFatp(const unsigned long int) StcFatp(const u64);               \
    typedef StcFatp(const unsigned long long int) StcFatp(const usize);        \
                                                                               \
    typedef StcFatp(signed char const) StcFatp(i8 const);                      \
    typedef StcFatp(signed short int const) StcFatp(i16 const);                \
    typedef StcFatp(signed int const) StcFatp(i32 const);                      \
    typedef StcFatp(signed long int const) StcFatp(i64 const);                 \
    typedef StcFatp(signed long long int const) StcFatp(isize const);          \
    typedef StcFatp(unsigned char const) StcFatp(u8 const);                    \
    typedef StcFatp(unsigned short int const) StcFatp(u16 const);              \
    typedef StcFatp(unsigned int const) StcFatp(u32 const);                    \
    typedef StcFatp(unsigned long int const) StcFatp(u64 const);               \
    typedef StcFatp(unsigned long long int const) StcFatp(usize const);        \
                                                                               \
    typedef StcFatp(signed char ptr) StcFatp(i8 ptr);                          \
    typedef StcFatp(signed short int ptr) StcFatp(i16 ptr);                    \
    typedef StcFatp(signed int ptr) StcFatp(i32 ptr);                          \
    typedef StcFatp(signed long int ptr) StcFatp(i64 ptr);                     \
    typedef StcFatp(signed long long int ptr) StcFatp(isize ptr);              \
    typedef StcFatp(unsigned char ptr) StcFatp(u8 ptr);                        \
    typedef StcFatp(unsigned short int ptr) StcFatp(u16 ptr);                  \
    typedef StcFatp(unsigned int ptr) StcFatp(u32 ptr);                        \
    typedef StcFatp(unsigned long int ptr) StcFatp(u64 ptr);                   \
    typedef StcFatp(unsigned long long int ptr) StcFatp(usize ptr);            \
                                                                               \
    typedef StcFatp(const signed char ptr) StcFatp(const i8 ptr);              \
    typedef StcFatp(const signed short int ptr) StcFatp(const i16 ptr);        \
    typedef StcFatp(const signed int ptr) StcFatp(const i32 ptr);              \
    typedef StcFatp(const signed long int ptr) StcFatp(const i64 ptr);         \
    typedef StcFatp(const signed long long int ptr) StcFatp(const isize ptr);  \
    typedef StcFatp(const unsigned char ptr) StcFatp(const u8 ptr);            \
    typedef StcFatp(const unsigned short int ptr) StcFatp(const u16 ptr);      \
    typedef StcFatp(const unsigned int ptr) StcFatp(const u32 ptr);            \
    typedef StcFatp(const unsigned long int ptr) StcFatp(const u64 ptr);       \
    typedef StcFatp(const unsigned long long int ptr)                          \
        StcFatp(const usize ptr);                                              \
                                                                               \
    typedef StcFatp(signed char const ptr) StcFatp(i8 const ptr);              \
    typedef StcFatp(signed short int const ptr) StcFatp(i16 const ptr);        \
    typedef StcFatp(signed int const ptr) StcFatp(i32 const ptr);              \
    typedef StcFatp(signed long int const ptr) StcFatp(i64 const ptr);         \
    typedef StcFatp(signed long long int const ptr) StcFatp(isize const ptr);  \
    typedef StcFatp(unsigned char const ptr) StcFatp(u8 const ptr);            \
    typedef StcFatp(unsigned short int const ptr) StcFatp(u16 const ptr);      \
    typedef StcFatp(unsigned int const ptr) StcFatp(u32 const ptr);            \
    typedef StcFatp(unsigned long int const ptr) StcFatp(u64 const ptr);       \
    typedef StcFatp(unsigned long long int const ptr)                          \
        StcFatp(usize const ptr);                                              \
                                                                               \
    typedef StcFatp(signed char ptr const) StcFatp(i8 ptr const);              \
    typedef StcFatp(signed short int ptr const) StcFatp(i16 ptr const);        \
    typedef StcFatp(signed int ptr const) StcFatp(i32 ptr const);              \
    typedef StcFatp(signed long int ptr const) StcFatp(i64 ptr const);         \
    typedef StcFatp(signed long long int ptr const) StcFatp(isize ptr const);  \
    typedef StcFatp(unsigned char ptr const) StcFatp(u8 ptr const);            \
    typedef StcFatp(unsigned short int ptr const) StcFatp(u16 ptr const);      \
    typedef StcFatp(unsigned int ptr const) StcFatp(u32 ptr const);            \
    typedef StcFatp(unsigned long int ptr const) StcFatp(u64 ptr const);       \
    typedef StcFatp(unsigned long long int ptr const)                          \
        StcFatp(usize ptr const);                                              \
                                                                               \
    typedef StcFatp(const signed char ptr const) StcFatp(const i8 ptr const);  \
    typedef StcFatp(const signed short int ptr const)                          \
        StcFatp(const i16 ptr const);                                          \
    typedef StcFatp(const signed int ptr const) StcFatp(const i32 ptr const);  \
    typedef StcFatp(const signed long int ptr const)                           \
        StcFatp(const i64 ptr const);                                          \
    typedef StcFatp(const signed long long int ptr const)                      \
        StcFatp(const isize ptr const);                                        \
    typedef StcFatp(const unsigned char ptr const)                             \
        StcFatp(const u8 ptr const);                                           \
    typedef StcFatp(const unsigned short int ptr const)                        \
        StcFatp(const u16 ptr const);                                          \
    typedef StcFatp(const unsigned int ptr const)                              \
        StcFatp(const u32 ptr const);                                          \
    typedef StcFatp(const unsigned long int ptr const)                         \
        StcFatp(const u64 ptr const);                                          \
    typedef StcFatp(const unsigned long long int ptr const)                    \
        StcFatp(const usize ptr const);                                        \
                                                                               \
    typedef StcFatp(signed char const ptr const) StcFatp(i8 const ptr const);  \
    typedef StcFatp(signed short int const ptr const)                          \
        StcFatp(i16 const ptr const);                                          \
    typedef StcFatp(signed int const ptr const) StcFatp(i32 const ptr const);  \
    typedef StcFatp(signed long int const ptr const)                           \
        StcFatp(i64 const ptr const);                                          \
    typedef StcFatp(signed long long int const ptr const)                      \
        StcFatp(isize const ptr const);                                        \
    typedef StcFatp(unsigned char const ptr const)                             \
        StcFatp(u8 const ptr const);                                           \
    typedef StcFatp(unsigned short int const ptr const)                        \
        StcFatp(u16 const ptr const);                                          \
    typedef StcFatp(unsigned int const ptr const)                              \
        StcFatp(u32 const ptr const);                                          \
    typedef StcFatp(unsigned long int const ptr const)                         \
        StcFatp(u64 const ptr const);                                          \
    typedef StcFatp(unsigned long long int const ptr const)                    \
        StcFatp(usize const ptr const);                                        \
                                                                               \
    typedef StcFatp(signed char ptr ptr) StcFatp(i8 ptr ptr);                  \
    typedef StcFatp(signed short int ptr ptr) StcFatp(i16 ptr ptr);            \
    typedef StcFatp(signed int ptr ptr) StcFatp(i32 ptr ptr);                  \
    typedef StcFatp(signed long int ptr ptr) StcFatp(i64 ptr ptr);             \
    typedef StcFatp(signed long long int ptr ptr) StcFatp(isize ptr ptr);      \
    typedef StcFatp(unsigned char ptr ptr) StcFatp(u8 ptr ptr);                \
    typedef StcFatp(unsigned short int ptr ptr) StcFatp(u16 ptr ptr);          \
    typedef StcFatp(unsigned int ptr ptr) StcFatp(u32 ptr ptr);                \
    typedef StcFatp(unsigned long int ptr ptr) StcFatp(u64 ptr ptr);           \
    typedef StcFatp(unsigned long long int ptr ptr) StcFatp(usize ptr ptr);    \
                                                                               \
    typedef StcFatp(const signed char ptr ptr) StcFatp(const i8 ptr ptr);      \
    typedef StcFatp(const signed short int ptr ptr)                            \
        StcFatp(const i16 ptr ptr);                                            \
    typedef StcFatp(const signed int ptr ptr) StcFatp(const i32 ptr ptr);      \
    typedef StcFatp(const signed long int ptr ptr) StcFatp(const i64 ptr ptr); \
    typedef StcFatp(const signed long long int ptr ptr)                        \
        StcFatp(const isize ptr ptr);                                          \
    typedef StcFatp(const unsigned char ptr ptr) StcFatp(const u8 ptr ptr);    \
    typedef StcFatp(const unsigned short int ptr ptr)                          \
        StcFatp(const u16 ptr ptr);                                            \
    typedef StcFatp(const unsigned int ptr ptr) StcFatp(const u32 ptr ptr);    \
    typedef StcFatp(const unsigned long int ptr ptr)                           \
        StcFatp(const u64 ptr ptr);                                            \
    typedef StcFatp(const unsigned long long int ptr ptr)                      \
        StcFatp(const usize ptr ptr);                                          \
                                                                               \
    typedef StcFatp(signed char const ptr ptr) StcFatp(i8 const ptr ptr);      \
    typedef StcFatp(signed short int const ptr ptr)                            \
        StcFatp(i16 const ptr ptr);                                            \
    typedef StcFatp(signed int const ptr ptr) StcFatp(i32 const ptr ptr);      \
    typedef StcFatp(signed long int const ptr ptr) StcFatp(i64 const ptr ptr); \
    typedef StcFatp(signed long long int const ptr ptr)                        \
        StcFatp(isize const ptr ptr);                                          \
    typedef StcFatp(unsigned char const ptr ptr) StcFatp(u8 const ptr ptr);    \
    typedef StcFatp(unsigned short int const ptr ptr)                          \
        StcFatp(u16 const ptr ptr);                                            \
    typedef StcFatp(unsigned int const ptr ptr) StcFatp(u32 const ptr ptr);    \
    typedef StcFatp(unsigned long int const ptr ptr)                           \
        StcFatp(u64 const ptr ptr);                                            \
    typedef StcFatp(unsigned long long int const ptr ptr)                      \
        StcFatp(usize const ptr ptr);                                          \
                                                                               \
    typedef StcFatp(signed char ptr const ptr) StcFatp(i8 ptr const ptr);      \
    typedef StcFatp(signed short int ptr const ptr)                            \
        StcFatp(i16 ptr const ptr);                                            \
    typedef StcFatp(signed int ptr const ptr) StcFatp(i32 ptr const ptr);      \
    typedef StcFatp(signed long int ptr const ptr) StcFatp(i64 ptr const ptr); \
    typedef StcFatp(signed long long int ptr const ptr)                        \
        StcFatp(isize ptr const ptr);                                          \
    typedef StcFatp(unsigned char ptr const ptr) StcFatp(u8 ptr const ptr);    \
    typedef StcFatp(unsigned short int ptr const ptr)                          \
        StcFatp(u16 ptr const ptr);                                            \
    typedef StcFatp(unsigned int ptr const ptr) StcFatp(u32 ptr const ptr);    \
    typedef StcFatp(unsigned long int ptr const ptr)                           \
        StcFatp(u64 ptr const ptr);                                            \
    typedef StcFatp(unsigned long long int ptr const ptr)                      \
        StcFatp(usize ptr const ptr);                                          \
                                                                               \
    typedef StcFatp(const signed char ptr const ptr)                           \
        StcFatp(const i8 ptr const ptr);                                       \
    typedef StcFatp(const signed short int ptr const ptr)                      \
        StcFatp(const i16 ptr const ptr);                                      \
    typedef StcFatp(const signed int ptr const ptr)                            \
        StcFatp(const i32 ptr const ptr);                                      \
    typedef StcFatp(const signed long int ptr const ptr)                       \
        StcFatp(const i64 ptr const ptr);                                      \
    typedef StcFatp(const signed long long int ptr const ptr)                  \
        StcFatp(const isize ptr const ptr);                                    \
    typedef StcFatp(const unsigned char ptr const ptr)                         \
        StcFatp(const u8 ptr const ptr);                                       \
    typedef StcFatp(const unsigned short int ptr const ptr)                    \
        StcFatp(const u16 ptr const ptr);                                      \
    typedef StcFatp(const unsigned int ptr const ptr)                          \
        StcFatp(const u32 ptr const ptr);                                      \
    typedef StcFatp(const unsigned long int ptr const ptr)                     \
        StcFatp(const u64 ptr const ptr);                                      \
    typedef StcFatp(const unsigned long long int ptr const ptr)                \
        StcFatp(const usize ptr const ptr);                                    \
                                                                               \
    typedef StcFatp(signed char const ptr const ptr)                           \
        StcFatp(i8 const ptr const ptr);                                       \
    typedef StcFatp(signed short int const ptr const ptr)                      \
        StcFatp(i16 const ptr const ptr);                                      \
    typedef StcFatp(signed int const ptr const ptr)                            \
        StcFatp(i32 const ptr const ptr);                                      \
    typedef StcFatp(signed long int const ptr const ptr)                       \
        StcFatp(i64 const ptr const ptr);                                      \
    typedef StcFatp(signed long long int const ptr const ptr)                  \
        StcFatp(isize const ptr const ptr);                                    \
    typedef StcFatp(unsigned char const ptr const ptr)                         \
        StcFatp(u8 const ptr const ptr);                                       \
    typedef StcFatp(unsigned short int const ptr const ptr)                    \
        StcFatp(u16 const ptr const ptr);                                      \
    typedef StcFatp(unsigned int const ptr const ptr)                          \
        StcFatp(u32 const ptr const ptr);                                      \
    typedef StcFatp(unsigned long int const ptr const ptr)                     \
        StcFatp(u64 const ptr const ptr);                                      \
    typedef StcFatp(unsigned long long int const ptr const ptr)                \
        StcFatp(usize const ptr const ptr);                                    \
                                                                               \
    typedef StcFatp(signed char ptr ptr const) StcFatp(i8 ptr ptr const);      \
    typedef StcFatp(signed short int ptr ptr const)                            \
        StcFatp(i16 ptr ptr const);                                            \
    typedef StcFatp(signed int ptr ptr const) StcFatp(i32 ptr ptr const);      \
    typedef StcFatp(signed long int ptr ptr const) StcFatp(i64 ptr ptr const); \
    typedef StcFatp(signed long long int ptr ptr const)                        \
        StcFatp(isize ptr ptr const);                                          \
    typedef StcFatp(unsigned char ptr ptr const) StcFatp(u8 ptr ptr const);    \
    typedef StcFatp(unsigned short int ptr ptr const)                          \
        StcFatp(u16 ptr ptr const);                                            \
    typedef StcFatp(unsigned int ptr ptr const) StcFatp(u32 ptr ptr const);    \
    typedef StcFatp(unsigned long int ptr ptr const)                           \
        StcFatp(u64 ptr ptr const);                                            \
    typedef StcFatp(unsigned long long int ptr ptr const)                      \
        StcFatp(usize ptr ptr const);                                          \
                                                                               \
    typedef StcFatp(const signed char ptr ptr const)                           \
        StcFatp(const i8 ptr ptr const);                                       \
    typedef StcFatp(const signed short int ptr ptr const)                      \
        StcFatp(const i16 ptr ptr const);                                      \
    typedef StcFatp(const signed int ptr ptr const)                            \
        StcFatp(const i32 ptr ptr const);                                      \
    typedef StcFatp(const signed long int ptr ptr const)                       \
        StcFatp(const i64 ptr ptr const);                                      \
    typedef StcFatp(const signed long long int ptr ptr const)                  \
        StcFatp(const isize ptr ptr const);                                    \
    typedef StcFatp(const unsigned char ptr ptr const)                         \
        StcFatp(const u8 ptr ptr const);                                       \
    typedef StcFatp(const unsigned short int ptr ptr const)                    \
        StcFatp(const u16 ptr ptr const);                                      \
    typedef StcFatp(const unsigned int ptr ptr const)                          \
        StcFatp(const u32 ptr ptr const);                                      \
    typedef StcFatp(const unsigned long int ptr ptr const)                     \
        StcFatp(const u64 ptr ptr const);                                      \
    typedef StcFatp(const unsigned long long int ptr ptr const)                \
        StcFatp(const usize ptr ptr const);                                    \
                                                                               \
    typedef StcFatp(signed char const ptr ptr const)                           \
        StcFatp(i8 const ptr ptr const);                                       \
    typedef StcFatp(signed short int const ptr ptr const)                      \
        StcFatp(i16 const ptr ptr const);                                      \
    typedef StcFatp(signed int const ptr ptr const)                            \
        StcFatp(i32 const ptr ptr const);                                      \
    typedef StcFatp(signed long int const ptr ptr const)                       \
        StcFatp(i64 const ptr ptr const);                                      \
    typedef StcFatp(signed long long int const ptr ptr const)                  \
        StcFatp(isize const ptr ptr const);                                    \
    typedef StcFatp(unsigned char const ptr ptr const)                         \
        StcFatp(u8 const ptr ptr const);                                       \
    typedef StcFatp(unsigned short int const ptr ptr const)                    \
        StcFatp(u16 const ptr ptr const);                                      \
    typedef StcFatp(unsigned int const ptr ptr const)                          \
        StcFatp(u32 const ptr ptr const);                                      \
    typedef StcFatp(unsigned long int const ptr ptr const)                     \
        StcFatp(u64 const ptr ptr const);                                      \
    typedef StcFatp(unsigned long long int const ptr ptr const)                \
        StcFatp(usize const ptr ptr const);                                    \
                                                                               \
    typedef StcFatp(signed char ptr const ptr const)                           \
        StcFatp(i8 ptr const ptr const);                                       \
    typedef StcFatp(signed short int ptr const ptr const)                      \
        StcFatp(i16 ptr const ptr const);                                      \
    typedef StcFatp(signed int ptr const ptr const)                            \
        StcFatp(i32 ptr const ptr const);                                      \
    typedef StcFatp(signed long int ptr const ptr const)                       \
        StcFatp(i64 ptr const ptr const);                                      \
    typedef StcFatp(signed long long int ptr const ptr const)                  \
        StcFatp(isize ptr const ptr const);                                    \
    typedef StcFatp(unsigned char ptr const ptr const)                         \
        StcFatp(u8 ptr const ptr const);                                       \
    typedef StcFatp(unsigned short int ptr const ptr const)                    \
        StcFatp(u16 ptr const ptr const);                                      \
    typedef StcFatp(unsigned int ptr const ptr const)                          \
        StcFatp(u32 ptr const ptr const);                                      \
    typedef StcFatp(unsigned long int ptr const ptr const)                     \
        StcFatp(u64 ptr const ptr const);                                      \
    typedef StcFatp(unsigned long long int ptr const ptr const)                \
        StcFatp(usize ptr const ptr const);                                    \
                                                                               \
    typedef StcFatp(const signed char ptr const ptr const)                     \
        StcFatp(const i8 ptr const ptr const);                                 \
    typedef StcFatp(const signed short int ptr const ptr const)                \
        StcFatp(const i16 ptr const ptr const);                                \
    typedef StcFatp(const signed int ptr const ptr const)                      \
        StcFatp(const i32 ptr const ptr const);                                \
    typedef StcFatp(const signed long int ptr const ptr const)                 \
        StcFatp(const i64 ptr const ptr const);                                \
    typedef StcFatp(const signed long long int ptr const ptr const)            \
        StcFatp(const isize ptr const ptr const);                              \
    typedef StcFatp(const unsigned char ptr const ptr const)                   \
        StcFatp(const u8 ptr const ptr const);                                 \
    typedef StcFatp(const unsigned short int ptr const ptr const)              \
        StcFatp(const u16 ptr const ptr const);                                \
    typedef StcFatp(const unsigned int ptr const ptr const)                    \
        StcFatp(const u32 ptr const ptr const);                                \
    typedef StcFatp(const unsigned long int ptr const ptr const)               \
        StcFatp(const u64 ptr const ptr const);                                \
    typedef StcFatp(const unsigned long long int ptr const ptr const)          \
        StcFatp(const usize ptr const ptr const);                              \
                                                                               \
    typedef StcFatp(signed char const ptr const ptr const)                     \
        StcFatp(i8 const ptr const ptr const);                                 \
    typedef StcFatp(signed short int const ptr const ptr const)                \
        StcFatp(i16 const ptr const ptr const);                                \
    typedef StcFatp(signed int const ptr const ptr const)                      \
        StcFatp(i32 const ptr const ptr const);                                \
    typedef StcFatp(signed long int const ptr const ptr const)                 \
        StcFatp(i64 const ptr const ptr const);                                \
    typedef StcFatp(signed long long int const ptr const ptr const)            \
        StcFatp(isize const ptr const ptr const);                              \
    typedef StcFatp(unsigned char const ptr const ptr const)                   \
        StcFatp(u8 const ptr const ptr const);                                 \
    typedef StcFatp(unsigned short int const ptr const ptr const)              \
        StcFatp(u16 const ptr const ptr const);                                \
    typedef StcFatp(unsigned int const ptr const ptr const)                    \
        StcFatp(u32 const ptr const ptr const);                                \
    typedef StcFatp(unsigned long int const ptr const ptr const)               \
        StcFatp(u64 const ptr const ptr const);                                \
    typedef StcFatp(unsigned long long int const ptr const ptr const)          \
        StcFatp(usize const ptr const ptr const)

#endif /* STC_FATP_UTIL_H */
