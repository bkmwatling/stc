#ifndef STC_MACRO_H
#define STC_MACRO_H

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_MACRO_ENABLE_SHORT_NAMES)
#    define STRINGIFY STC_STRINGIFY
#    define CONCAT    STC_CONCAT

#    define WITH      STC_WITH
#    define DEFER     STC_DEFER
#    define TYPECHECK STC_TYPECHECK
#endif /* STC_MACRO_ENABLE_SHORT_NAMES */

/* NOTE: Macro indirection recommended due to #s and a##b */
#define __STC_STRINGIFY(s) #s
#define STC_STRINGIFY(s)   __STC_STRINGIFY(s)
#define __STC_CONCAT(a, b) a##b
#define STC_CONCAT(a, b)   __STC_CONCAT(a, b)

#define _STC_MACRO_VAR(x) STC_CONCAT(x, __LINE__)

#define STC_WITH(begin, end)                                         \
    for (int _STC_MACRO_VAR(_i_) = (begin, 0); !_STC_MACRO_VAR(_i_); \
         _STC_MACRO_VAR(_i_)++, end)

#define STC_DEFER(end)                                      \
    for (int _STC_MACRO_VAR(_i_) = 0; !_STC_MACRO_VAR(_i_); \
         _STC_MACRO_VAR(_i_)++, end)

#define STC_TYPECHECK(T, x)                                    \
    ({                                                         \
        T             _STC_MACRO_VAR(_x_);                     \
        __typeof__(x) _STC_MACRO_VAR(_y_);                     \
        (void) (&_STC_MACRO_VAR(_x_) == &_STC_MACRO_VAR(_y_)); \
    })

/* --- Common macros for doing funky macro magic ---------------------------- */

#define _STC_FIRST(a, ...) a

/**
 * Used to detect comma by relying on shifted argmuents if there is one (see
 * STC_IF_HAS_COMMA macro), but must invoked in only one specific way (again see
 * STC_IF_HAS_COMMA macro) where you have STC_ARGn(_0, _1, _2, up to _n, ...) _n
 * where n is the maximum number of variadic arguments to support. The
 * invokation is then STC_ARGn(__VA_ARGS__, t, t, t, repeated (n - 1) times
 * total, f, f), where t is the truth value, and f is the false value.
 */
#define _STC_ARG50(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
                   _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, \
                   _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, \
                   _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
                   _50, ...)                                                   \
    _50

/* for use with STC_IF_HAS_COMMA or STC_HAS_COMMA */
#define _STC_COMMA(...) ,

/* detects if the arguments contains a comma (not nested in parentheses) by
 * replacing with either f (no comma) or t (has comma) */
#define _STC_IF_HAS_COMMA(t, f, ...)                                           \
    _STC_ARG50(__VA_ARGS__, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, \
               t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t, t,  \
               t, t, t, t, t, t, t, t, t, t, t, f, f)

#define _STC_HAS_COMMA(...) _STC_IF_HAS_COMMA(1, 0, __VA_ARGS__)

#define _STC_IFEMPTY(t, f, ...) \
    _STC_IF_HAS_COMMA(t, f, __STC_EMPTY(__VA_ARGS__))

#define _STC_ISEMPTY(...) _STC_HAS_COMMA(__STC_EMPTY(__VA_ARGS__))

/* check if variadic arguments is empty by checking main case (last one) and
 * three other corner cases */
#define __STC_EMPTY(...)                                   \
    __STC_EMPTY_(_STC_HAS_COMMA(__VA_ARGS__),              \
                 _STC_HAS_COMMA(_STC_COMMA __VA_ARGS__),   \
                 _STC_HAS_COMMA(__VA_ARGS__(/* empty */)), \
                 _STC_HAS_COMMA(_STC_COMMA __VA_ARGS__(/* empty */)))
/* simple macro to concatenate arguments */
#define __STC_PASTES(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4
/* use above macro to convert four cases into output for STC_HAS_COMMA */
#define __STC_EMPTY_(_0, _1, _2, _3) \
    __STC_PASTES(__STC_EMPTY_CASE_, _0, _1, _2, _3)
/* place comma in correct case from above to trigger STC_HAS_COMMA to output 1,
 * and with other cases not defined they will be replaced with nothing making
 * STC_HAS_COMMA output 0 */
#define __STC_EMPTY_CASE_0001 ,

// apply function f for each argument recursively with tail
#define __STC_FOREACH_1(f, a)       f(a, )
#define __STC_FOREACH_2(f, a, ...)  f(a, __STC_FOREACH_1(f, __VA_ARGS__))
#define __STC_FOREACH_3(f, a, ...)  f(a, __STC_FOREACH_2(f, __VA_ARGS__))
#define __STC_FOREACH_4(f, a, ...)  f(a, __STC_FOREACH_3(f, __VA_ARGS__))
#define __STC_FOREACH_5(f, a, ...)  f(a, __STC_FOREACH_4(f, __VA_ARGS__))
#define __STC_FOREACH_6(f, a, ...)  f(a, __STC_FOREACH_5(f, __VA_ARGS__))
#define __STC_FOREACH_7(f, a, ...)  f(a, __STC_FOREACH_6(f, __VA_ARGS__))
#define __STC_FOREACH_8(f, a, ...)  f(a, __STC_FOREACH_7(f, __VA_ARGS__))
#define __STC_FOREACH_9(f, a, ...)  f(a, __STC_FOREACH_8(f, __VA_ARGS__))
#define __STC_FOREACH_10(f, a, ...) f(a, __STC_FOREACH_9(f, __VA_ARGS__))
#define __STC_FOREACH(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) \
    STC_CONCAT(__STC_FOREACH_, N)
#define _STC_FOREACH(f, ...) \
    __STC_FOREACH(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)(f, __VA_ARGS__)

/* --- End of common macro magic macros ------------------------------------- */

/* --- Macro magic for dealing with __VA_ARGS__ potentially being empty ----- */

/* taken from
 * t6847kimo.github.io/blog/2019/02/04/Remove-comma-in-variadic-macro.html
 */

/**
 * Take __VA_ARGS__ and prepend a comma if it is not empty; else blank.
 * USAGE: #define PRINTF(fmt, ...) printf(fmt _STC_PREPEND_COMMA(__VA_ARGS__))
 */
#define _STC_PREPEND_COMMA(...) \
    __STC_PREPEND_COMMA_IF_NOT_EMPTY(_STC_ISEMPTY(__VA_ARGS__), __VA_ARGS__)

/**
 * Take __VA_ARGS__ and append a comma if it is not empty; else blank.
 * USAGE: #define NULL_TERMINATE(...) func(_STC_APPEND_COMMA(__VA_ARGS__) NULL)
 */
#define _STC_APPEND_COMMA(...) \
    __STC_APPEND_COMMA_IF_NOT_EMPTY(_STC_ISEMPTY(__VA_ARGS__), __VA_ARGS__)

/* use a layer of macro indirection to ensure _STC_ISEMPTY is evaluated */
#define __STC_PREPEND_COMMA_IF_NOT_EMPTY(is_empty, ...) \
    __STC_PREPEND_COMMA_IF_NOT_EMPTY_EXPAND_CHECK_EMPTY(is_empty, __VA_ARGS__)

/* append result of _STC_ISEMPTY check to choose correct macro depending on
 * whether __VA_ARGS__ is empty or not */
#define __STC_PREPEND_COMMA_IF_NOT_EMPTY_EXPAND_CHECK_EMPTY(is_empty, ...) \
    __STC_PREPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_##is_empty(__VA_ARGS__)
#define __STC_PREPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_0(...) , __VA_ARGS__
#define __STC_PREPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_1(...)

/* use a layer of macro indirection to ensure _STC_ISEMPTY is evaluated */
#define __STC_APPEND_COMMA_IF_NOT_EMPTY(is_empty, ...) \
    __STC_APPEND_COMMA_IF_NOT_EMPTY_EXPAND_CHECK_EMPTY(is_empty, __VA_ARGS__)

/* append result of _STC_ISEMPTY check to choose correct macro depending on
 * whether __VA_ARGS__ is empty or not */
#define __STC_APPEND_COMMA_IF_NOT_EMPTY_EXPAND_CHECK_EMPTY(is_empty, ...) \
    __STC_APPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_##is_empty(__VA_ARGS__)
#define __STC_APPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_0(...) __VA_ARGS__,
#define __STC_APPEND_COMMA_IF_NOT_EMPTY_EXPAND_IS_EMPTY_1(...)

/* --- End of __VA_ARGS__ macro magic --------------------------------------- */

#endif /* STC_MACRO_H */
