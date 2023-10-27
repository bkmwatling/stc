#ifndef STC_COMMON_H
#define STC_COMMON_H

#include <stdlib.h>

/*** Enable short names for macros and functions ******************************/

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_COMMON_ENABLE_SHORT_NAMES)
#    ifndef STC_DISABLE_ASSERT
#        define assert     stc_assert
#        define assert_msg stc_assert_msg
#    endif

#    define EPRINTF STC_EPRINTF
#    define INFO    STC_INFO
#    define FINFO   STC_FINFO
#    define WARN    STC_WARN
#    define FWARN   STC_FWARN
#    define ERROR   STC_ERROR
#    define FERROR  STC_FERROR
#    define FATAL   STC_FATAL
#    define FFATAL  STC_FFATAL
#    define DEBUG   STC_DEBUG
#    define FDEBUG  STC_FDEBUG

#    define STRINGIFY STC_STRINGIFY
#    define GLUE      STC_GLUE

#    define UNUSED            STC_UNUSED
#    define UNIMPLEMENTED     STC_UNIMPLEMENTED
#    define UNIMPLEMENTED_MSG STC_UNIMPLEMENTED_MSG
#    define TODO              STC_TODO
#    define TODO_MSG          STC_TODO_MSG

#    define ARRAY_COUNT   STC_ARRAY_COUNT
#    define INT_FROM_PTR  STC_INT_FROM_PTR
#    define PTR_FROM_INT  STC_PTR_FROM_INT
#    define PTR_TO_INT    STC_PTR_TO_INT
#    define INT_TO_PTR    STC_INT_TO_PTR
#    define MEMBER        STC_MEMBER
#    define MEMBER_OFFSET STC_MEMBER_OFFSET

#    define MIN             STC_MIN
#    define MAX             STC_MAX
#    define CLAMP           STC_CLAMP
#    define CLAMP_TOP       STC_CLAMP_TOP
#    define CLAMP_BOT       STC_CLAMP_BOT
#    define ALIGN_UP_POW2   STC_ALIGN_UP_POW2
#    define ALIGN_DOWN_POW2 STC_ALIGN_DOWN_POW2

#    define GLOBAL STC_GLOBAL
#    define LOCAL  STC_LOCAL
#    define FUNC   STC_FUNC
#endif /* STC_COMMON_ENABLE_SHORT_NAMES */

/*** Compiler and OS detection ************************************************/

#if defined(__clang__)
#    define COMPILER_CLANG

#    if defined(_WIN32)
#        define OS_WINDOWS
#    elif defined(__gnu_linux__)
#        define OS_LINUX
#    elif defined(__APPLE__) && defined(__MACH__)
#        define OS_MAC
#    else
#        error "missing OS detection"
#    endif

#    if defined(__amd64__)
#        define ARCH_X64
#    elif defined(__i386__)
#        define ARCH_X86
#    elif defined(__arm__)
#        define ARCH_ARM
#    elif defined(__aarch64__)
#        define ARCH_ARM64
#    else
#        error "missing architecture detection"
#    endif

#elif defined(__GNUC__)
#    define COMPILER_GCC

#    if defined(__WIN32)
#        define OS_WINDOWS
#    elif defined(__gnu_linux__)
#        define OS_LINUX
#    elif defined(__APPLE__) && defined(__MACH__)
#        define OS_MAC
#    else
#        error "missing OS detection"
#    endif

#    if defined(__amd64__)
#        define ARCH_X64
#    elif defined(__i386__)
#        define ARCH_X86
#    elif defined(__arm__)
#        define ARCH_ARM
#    elif defined(__aarch64__)
#        define ARCH_ARM64
#    else
#        error "missing architecture detection"
#    endif

#elif defined(_MSC_VER)
#    define COMPILER_CL

#    if defined(_WIN32)
#        define OS_WINDOWS
#    else
#        error "missing OS detection"
#    endif

#    if defined(_M_AMD64)
#        define ARCH_X64
#    elif defined(_M_I86)
#        define ARCH_X86
#    elif defined(_M_ARM)
#        define ARCH_ARM
/* TODO: check for ARM64 */
#    else
#        error "missing architecture detection"
#    endif

#else
#    error "unable to detect compiler"
#endif /* defined(__clang__) */

/*** Helper macros ************************************************************/

#define STC_STATMENTS(stmts) \
    do {                     \
        stmts                \
    } while (0)

#ifndef STC_DISABLE_STDIO
#    include <stdio.h>

#    define STC_PRINTF               printf
#    define STC_FPRINTF(stream, ...) fprintf(stream, __VA_ARGS__)
#else
#    define STC_PRINTF(...)
#    define STC_FPRINTF(stream, ...)
#endif /* STC_DISABLE_STDIO */

#define STC_EPRINTF(...)        STC_FPRINTF(stderr, __VA_ARGS__)
#define STC_INFO(...)           STC_PRINTF("[INFO] " __VA_ARGS__)
#define STC_FINFO(stream, ...)  STC_FPRINTF(stream, "[INFO] " __VA_ARGS__)
#define STC_WARN(...)           STC_EPRINTF("[WARN] " __VA_ARGS__)
#define STC_FWARN(stream, ...)  STC_FPRINTF(stream, "[WARN] " __VA_ARGS__)
#define STC_ERROR(...)          STC_EPRINTF("[ERROR] " __VA_ARGS__)
#define STC_FERROR(stream, ...) STC_FPRINTF(stream, "[ERROR] " __VA_ARGS__)
#define STC_FATAL(...)          STC_STATMENTS(STC_ERROR(__VA_ARGS__); abort())
#define STC_FFATAL(stream, ...) \
    STC_STATMENTS(STC_FERROR(stream, __VA_ARGS__); abort())

#ifdef STC_ENABLE_DEBUG
#    define STC_DEBUG(...)          STC_EPRINTF("[DEBUG] " __VA_ARGS__)
#    define STC_FDEBUG(stream, ...) STC_FPRINTF(stream, "[DEBUG] " __VA_ARGS__)
#else
#    define STC_DEBUG(...)
#    define STC_FDEBUG(stream, ...)
#endif /* STC_DEBUG */

#ifdef STC_USE_STD_ASSERT
#    ifdef assert
#        undef assert
#    endif
#    include <assert.h>
#    define stc_assert(cond)          assert(cond)
#    define stc_assert_msg(cond, msg) assert((cond) && (msg))
#elif defined(STC_DISABLE_ASSERT)
#    define stc_assert(cond)
#    define stc_assert_msg(cond, msg)
#else
#    ifndef STC_ASSERT_BREAK
#        define STC_ASSERT_MSG(msg) \
            __FILE__ ":%d: Assertion failed: " msg "\n", __LINE__
#        define STC_ASSERT_BREAK(cond) \
            STC_STATMENTS(STC_EPRINTF(STC_ASSERT_MSG(#cond)); abort();)
#    endif

#    define stc_assert(cond) STC_STATMENTS(if (!(cond)) STC_ASSERT_BREAK(cond);)
#    define stc_assert_msg(cond, msg) \
        STC_STATMENTS(if (!(cond)) {  \
            STC_EPRINTF(msg "\n");    \
            STC_ASSERT_BREAK(cond);   \
        })
#endif /* STC_USE_STD_ASSERT */

/* NOTE: Macro indirection recommended due to #s and a##b */
#define __STC_STRINGIFY(s) #s
#define STC_STRINGIFY(s)   __STC_STRINGIFY(s)
#define __STC_GLUE(a, b)   a##b
#define STC_GLUE(a, b)     __STC_GLUE(a, b)

/* NOTE: "better" but doesn't work - ((void) (1 ? 0 : ((x), void(), 0))) */
#define STC_UNUSED(x)              ((void) &(x))
#define STC_UNIMPLEMENTED          stc_assert_msg(0, "Unimplemented")
#define STC_UNIMPLEMENTED_MSG(msg) stc_assert_msg(0, "Unimplemented: " msg)
#define STC_TODO                   stc_assert_msg(0, "TODO")
#define STC_TODO_MSG(msg)          stc_assert_msg(0, "TODO: " msg)

#define STC_ARRAY_COUNT(a) (sizeof(a) / sizeof(*(a)))

#define STC_INT_FROM_PTR(p) ((unsigned long long) ((char *) (p) - (char *) 0))
#define STC_PTR_FROM_INT(n) ((void *) ((char *) 0 + (n)))
#define STC_PTR_TO_INT      STC_INT_FROM_PTR
#define STC_INT_TO_PTR      STC_PTR_TO_INT

#define STC_MEMBER(T, m)        (((T *) 0)->m)
#define STC_MEMBER_OFFSET(T, m) STC_INT_FROM_PTR(&STC_MEMBER(T, m))

#define STC_MIN(x, y)       ((x) < (y) ? (x) : (y))
#define STC_MAX(x, y)       ((x) > (y) ? (x) : (y))
#define STC_CLAMP(x, a, b)  STC_CLAMP_TOP(STC_CLAMP_BOT((x), (a)), (b))
#define STC_CLAMP_TOP(x, y) STC_MIN((x), (y))
#define STC_CLAMP_BOT(x, y) STC_MAX((x), (y))

#define STC_ALIGN_UP_POW2(x, pow2)   (((x) + (pow2) -1) & ~((pow2) -1))
#define STC_ALIGN_DOWN_POW2(x, pow2) ((x) & ~((pow2) -1))

#define STC_GLOBAL static
#define STC_LOCAL  static
#define STC_FUNC   static

/*** Basic types **************************************************************/

#ifndef STC_DISABLE_BASIC_TYPES

#    ifdef STC_C99
#        include <stdint.h>
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef intmax_t  isize;
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef uintmax_t usize;
#    else
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long        i64;
typedef signed long long   isize;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long      u64;
typedef unsigned long long usize;
#    endif /* STC_C99 */

typedef u8     byte;
typedef float  f32;
typedef double f64;

typedef void void_func(void);

/*** Basic type constants as macros ***/

#    define I8_MIN  ((i8) 0x80)
#    define I16_MIN ((i16) 0x8000)
#    define I32_MIN ((i32) 0x80000000)
#    define I64_MIN ((i64) 0x8000000000000000llu)

#    define I8_MAX  ((i8) 0x7f)
#    define I16_MAX ((i16) 0x7fff)
#    define I32_MAX ((i32) 0x7fffffff)
#    define I64_MAX ((i64) 0x7fffffffffffffffllu)

#    define U8_MAX  0xff
#    define U16_MAX 0xffff
#    define U32_MAX 0xffffffff
#    define U64_MAX 0xffffffffffffffffllu

#    define F32_MACHINE_EPS 1.1920929e-7f
#    define F64_MACHINE_EPS 2.220446e-16

/*** Floating point standard and constant functions ***/

#    ifndef STC_DISABLE_FUNCTIONS
/**
 * Creates positive infinity as a 32-bit floating-point number.
 *
 * @return positive infinity in 32-bit floating-point
 */
f32 f32_inf(void);

/**
 * Creates negative infinity as a 32-bit floating-point number.
 *
 * @return negative infinity in 32-bit floating-point
 */
f32 f32_neg_inf(void);

/**
 * Creates positive infinity as a 64-bit floating-point number.
 *
 * @return positive infinity in 64-bit floating-point
 */
f64 f64_inf(void);

/**
 * Creates negative infinity as a 64-bit floating-point number.
 *
 * @return negative infinity in 64-bit floating-point
 */
f64 f64_neg_inf(void);
#    endif /* STC_DISABLE_FUNCTIONS */

#endif /* STC_DISABLE_BASIC_TYPES */

#endif /* STC_COMMON_H */
