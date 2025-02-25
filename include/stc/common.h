#ifndef STC_COMMON_H
#define STC_COMMON_H

#include <stdlib.h>

/* --- Enable short names for macros and functions -------------------------- */

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_COMMON_ENABLE_SHORT_NAMES)
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

#    define ASSERT     STC_ASSERT
#    define ASSERT_MSG STC_ASSERT_MSG

#    define UNUSED            STC_UNUSED
#    define UNIMPLEMENTED     STC_UNIMPLEMENTED
#    define UNIMPLEMENTED_MSG STC_UNIMPLEMENTED_MSG
#    define TODO              STC_TODO
#    define TODO_MSG          STC_TODO_MSG

#    define ARRAY_COUNT   STC_ARRAY_COUNT
#    define NUM_FROM_PTR  STC_NUM_FROM_PTR
#    define PTR_FROM_NUM  STC_PTR_FROM_NUM
#    define PTR_TO_NUM    STC_PTR_TO_NUM
#    define NUM_TO_PTR    STC_NUM_TO_PTR
#    define MEMBER        STC_MEMBER
#    define MEMBER_OFFSET STC_MEMBER_OFFSET

#    define MIN             STC_MIN
#    define MAX             STC_MAX
#    define CLAMP_TOP       STC_CLAMP_TOP
#    define CLAMP_BOT       STC_CLAMP_BOT
#    define CLAMP           STC_CLAMP
#    define ALIGN_UP_POW2   STC_ALIGN_UP_POW2
#    define ALIGN_DOWN_POW2 STC_ALIGN_DOWN_POW2

#    define GLOBAL STC_GLOBAL
#    define LOCAL  STC_LOCAL
#    define FUNC   STC_FUNC
#endif /* STC_COMMON_ENABLE_SHORT_NAMES */

/* --- Compiler and OS detection -------------------------------------------- */

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
#endif /* compiler checks */

/* --- Helper macros -------------------------------------------------------- */

#ifdef STC_FPRINTF
#    define STC_PRINTF(/* fmt, */...) \
        STC_FPRINTF(stdout, /* fmt, */ __VA_ARGS__)
#elif !defined(STC_DISABLE_STDIO)
#    include <stdio.h>

#    define STC_PRINTF(/* fmt, */...) printf(/* fmt, */ __VA_ARGS__)
#    define STC_FPRINTF(stream, /* fmt, */...) \
        fprintf(stream, /* fmt, */ __VA_ARGS__)
#else
#    define STC_PRINTF(/* fmt, */...)
#    define STC_FPRINTF(stream, /* fmt, */...)
#endif /* STC_FPRINTF */

#define STC_EPRINTF(/* fmt, */...) STC_FPRINTF(stderr, /* fmt, */ __VA_ARGS__)
#define STC_INFO(/* fmt, */...)    STC_PRINTF("[INFO] " /* fmt, */ __VA_ARGS__)
#define STC_FINFO(stream, /* fmt, */...) \
    STC_FPRINTF(stream, "[INFO] " /* fmt, */ __VA_ARGS__)
#define STC_WARN(/* fmt, */...) STC_EPRINTF("[WARN] " /* fmt, */ __VA_ARGS__)
#define STC_FWARN(stream, /* fmt, */...) \
    STC_FPRINTF(stream, "[WARN] " /* fmt, */ __VA_ARGS__)
#define STC_ERROR(/* fmt, */...) STC_EPRINTF("[ERROR] " /* fmt, */ __VA_ARGS__)
#define STC_FERROR(stream, /* fmt, */...) \
    STC_FPRINTF(stream, "[ERROR] " /* fmt, */ __VA_ARGS__)
#define STC_FATAL(/* fmt, */...)           \
    do {                                   \
        STC_ERROR(/* fmt, */ __VA_ARGS__); \
        abort();                           \
    } while (0)
#define STC_FFATAL(stream, /* fmt, */...)           \
    do {                                            \
        STC_FERROR(stream, /* fmt, */ __VA_ARGS__); \
        abort();                                    \
    } while (0)

#ifdef STC_ENABLE_DEBUG
#    define STC_DEBUG(/* fmt, */...) \
        STC_EPRINTF("[DEBUG] " /* fmt, */ __VA_ARGS__)
#    define STC_FDEBUG(stream, /* fmt, */...) \
        STC_FPRINTF(stream, "[DEBUG] " /* fmt, */ __VA_ARGS__)
#else
#    define STC_DEBUG(/* fmt, */...)
#    define STC_FDEBUG(stream, /* fmt, */...)
#endif /* STC_DEBUG */

#ifdef STC_DISABLE_ASSERT
#    define STC_ASSERT(cond)
#    define STC_ASSERT_MSG(cond, msg)
#elif defined(STC_USE_STD_ASSERT)
#    include <assert.h>
#    define STC_ASSERT(cond)          assert(cond)
#    define STC_ASSERT_MSG(cond, msg) assert((cond) && (msg))
#else
#    ifndef STC_ASSERT_BREAK
#        define _STC_ASSERT_MSG(msg) \
            __FILE__ ":%d: Assertion failed: " msg "\n", __LINE__
#        define STC_ASSERT_BREAK(cond)               \
            do {                                     \
                STC_EPRINTF(_STC_ASSERT_MSG(#cond)); \
                abort();                             \
            } while (0)
#    endif

#    define STC_ASSERT(cond)                         \
        do {                                         \
            if (!(cond)) { STC_ASSERT_BREAK(cond); } \
        } while (0)
#    define STC_ASSERT_MSG(cond, msg)   \
        do {                            \
            if (!(cond)) {              \
                STC_EPRINTF(msg "\n");  \
                STC_ASSERT_BREAK(cond); \
            }                           \
        } while (0)
#endif /* STC_DISABLE_ASSERT */

#define STC_UNUSED(x)              ((void) &(x))
#define STC_UNIMPLEMENTED          STC_ASSERT_MSG(0, "Unimplemented")
#define STC_UNIMPLEMENTED_MSG(msg) STC_ASSERT_MSG(0, "Unimplemented: " msg)
#define STC_TODO                   STC_ASSERT_MSG(0, "TODO")
#define STC_TODO_MSG(msg)          STC_ASSERT_MSG(0, "TODO: " msg)

#define STC_ARRAY_COUNT(a) (sizeof(a) / sizeof(*(a)))

#define STC_NUM_FROM_PTR(p) ((unsigned long long) ((char *) (p) - (char *) 0))
#define STC_PTR_FROM_NUM(n) ((void *) ((char *) 0 + (n)))
#define STC_PTR_TO_NUM      STC_NUM_FROM_PTR
#define STC_NUM_TO_PTR      STC_PTR_FROM_NUM

#define STC_MEMBER(T, m)        (((T *) 0)->m)
#define STC_MEMBER_OFFSET(T, m) STC_NUM_FROM_PTR(&STC_MEMBER(T, m))

#define STC_MIN(x, y)      ((x) < (y) ? (x) : (y))
#define STC_MAX(x, y)      ((x) > (y) ? (x) : (y))
#define STC_CLAMP_TOP      STC_MIN
#define STC_CLAMP_BOT      STC_MAX
#define STC_CLAMP(x, a, b) STC_CLAMP_TOP(STC_CLAMP_BOT(x, a), b)

#define STC_ALIGN_UP_POW2(x, pow2)   (((x) + (pow2) - 1) & ~((pow2) - 1))
#define STC_ALIGN_DOWN_POW2(x, pow2) ((x) & ~((pow2) - 1))

#define STC_GLOBAL static
#define STC_LOCAL  static
#define STC_FUNC   static

/* --- Basic types ---------------------------------------------------------- */

#ifndef STC_DISABLE_BASIC_TYPES

#    if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
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
#    endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

typedef u8     byte;
typedef float  f32;
typedef double f64;

typedef void void_func(void);

/* --- Basic type constants as macros --- */

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

/* --- Floating point standard and constant functions --- */

#    ifndef STC_DISABLE_FUNCTIONS
/**
 * Create positive infinity as a 32-bit floating-point number.
 *
 * @return positive infinity in 32-bit floating-point
 */
f32 f32_inf(void);

/**
 * Create negative infinity as a 32-bit floating-point number.
 *
 * @return negative infinity in 32-bit floating-point
 */
f32 f32_neg_inf(void);

/**
 * Create positive infinity as a 64-bit floating-point number.
 *
 * @return positive infinity in 64-bit floating-point
 */
f64 f64_inf(void);

/**
 * Create negative infinity as a 64-bit floating-point number.
 *
 * @return negative infinity in 64-bit floating-point
 */
f64 f64_neg_inf(void);
#    endif /* STC_DISABLE_FUNCTIONS */

#endif /* STC_DISABLE_BASIC_TYPES */

#endif /* STC_COMMON_H */
