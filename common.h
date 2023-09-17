#ifndef STC_COMMON_H
#define STC_COMMON_H

#include <stdlib.h>

/*** Enable short names for macros and functions ******************************/

/* Set all short names macros if STC_USE_SHORT_NAMES is set */
#ifdef STC_USE_SHORT_NAMES
#    ifndef STC_COMMON_USE_SHORT_NAMES
#        define STC_COMMON_USE_SHORT_NAMES
#    endif
#    ifndef STC_SLICE_USE_SHORT_NAMES
#        define STC_SLICE_USE_SHORT_NAMES
#    endif
#    ifndef STC_VEC_USE_SHORT_NAMES
#        define STC_VEC_USE_SHORT_NAMES
#    endif
#endif /* STC_USE_SHORT_NAMES */

#ifdef STC_COMMON_USE_SHORT_NAMES
#    ifndef STC_DISABLE_ASSERT
#        define assert stc_assert
#    endif
#    define STRINGIFY     STC_STRINGIFY
#    define GLUE          STC_GLUE
#    define ARRAY_COUNT   STC_ARRAY_COUNT
#    define INT_FROM_PTR  STC_INT_FROM_PTR
#    define PTR_FROM_INT  STC_PTR_FROM_INT
#    define MEMBER        STC_MEMBER
#    define MEMBER_OFFSET STC_MEMBER_OFFSET
#    define MIN           STC_MIN
#    define MAX           STC_MAX
#    define CLAMP         STC_CLAMP
#    define CLAMP_TOP     STC_CLAMP_TOP
#    define CLAMP_BOT     STC_CLAMP_BOT
#    define GLOBAL        STC_GLOBAL
#    define LOCAL         STC_LOCAL
#    define FUNC          STC_FUNC
#    ifndef STC_DISABLE_MEMORY_OPS
#        define memzero        stc_memzero
#        define MEMZERO_STRUCT STC_MEMZERO_STRUCT
#        define MEMZERO_ARRAY  STC_MEMZERO_ARRAY
#        define MEMZERO_TYPED  STC_MEMZERO_TYPED

#        define memeq         stc_memeq
#        define MEMCPY_STRUCT STC_MEMCPY_STRUCT
#        define MEMCPY_ARRAY  STC_MEMCPY_ARRAY
#        define MEMCPY_TYPED  STC_MEMCPY_TYPED
#    endif
#endif /* STC_COMMON_USE_SHORT_NAMES */

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

#define STC_STMTS(stmts) \
    do {                 \
        stmts            \
    } while (0)

#ifndef STC_DISABLE_STDIO
#    include <stdio.h>
#    define STC_PRINTF(...)  printf(__VA_ARGS__)
#    define STC_EPRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#    define STC_PRINTF(...)
#    define STC_EPRINTF(...)
#endif /* STC_DISABLE_STDIO */

#ifdef STC_USE_STD_ASSERT
#    ifdef assert
#        undef assert
#    endif
#    include <assert.h>
#    define stc_assert(cond) assert(cond)
#elif defined(STC_DISABLE_ASSERT)
#    define stc_assert(cond)
#else
#    ifndef STC_ASSERT_BREAK
#        define STC_ASSERT_BREAK(cond) \
            STC_EPRINTF(#cond);        \
            (*(int *) 0 = 0)
#    endif

#    define stc_assert(cond) \
        STC_STMNTS(if (!(cond)) { STC_ASSERT_BREAK(cond); })
#endif /* STC_USE_STD_ASSERT */

#define __STC_STRINGIFY(s) #s
#define STC_STRINGIFY(s)   __STC_STRINGIFY(s)
#define __STC_GLUE(a, b)   a##b
#define STC_GLUE(a, b)     __STC_GLUE(a, b)

#define STC_UNUSED(x) ((void) (x))
#define STC_UNIMPLEMENTED(msg) \
    STC_PRINTF(msg "\n");      \
    exit(1)

#define STC_ARRAY_COUNT(a) (sizeof(a) / sizeof(*(a)))

#define STC_INT_FROM_PTR(p) ((unsigned long long) ((char *) p - (char *) 0))
#define STC_PTR_FROM_INT(n) ((void *) ((char *) 0 + (n)))

#define STC_MEMBER(T, m)        (((T *) 0)->m)
#define STC_MEMBER_OFFSET(T, m) STC_INT_FROM_PTR(&STC_MEMBER(T, m))

#define STC_MIN(x, y)       ((x) < (y) ? (x) : (y))
#define STC_MAX(x, y)       ((x) > (y) ? (x) : (y))
#define STC_CLAMP(x, a, b)  STC_CLAMP_TOP(STC_CLAMP_BOT(x, a), b)
#define STC_CLAMP_TOP(x, y) STC_MIN(x, y)
#define STC_CLAMP_BOT(x, y) STC_MAX(x, y)

#define STC_GLOBAL static
#define STC_LOCAL  static
#define STC_FUNC   static

#ifndef STC_DISABLE_MEMORY_OPS
#    include <string.h>
#    define stc_memzero(p, size)      memset((p), 0, (size))
#    define STC_MEMZERO_STRUCT(p)     stc_memzero((p), sizeof(*(p)))
#    define STC_MEMZERO_ARRAY(p)      stc_memzero((p), sizeof(p))
#    define STC_MEMZERO_TYPED(p, len) stc_memzero((p), sizeof(*(p)) * (len))

#    define stc_memeq(x, y, size) (memcmp((x), (y), (size)) == 0)
#    define STC_MEMCPY_STRUCT(p, q) \
        memcpy((p), (q), STC_MIN(sizeof(*(p)), sizeof(*(q))))
#    define STC_MEMCPY_ARRAY(p, q) \
        memcpy((p), (q), STC_MIN(sizeof(p), sizeof(q)))
#    define STC_MEMCPY_TYPED(p, q, size) \
        memcpy((p), (q), STC_MIN(sizeof(*(p)), sizeof(*(q))) * (size))
#endif /* STC_DISABLE_MEMORY_OPS */

/*** Basic types **************************************************************/

#ifndef STC_DISABLE_BASIC_TYPES

#    ifdef STC_C99
#        include <stdint.h>
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef intmax_t  imax;
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef uintmax_t umax;
#    else
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long        i64;
typedef signed long long   imax;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long      u64;
typedef unsigned long long umax;
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

f32 f32_inf(void);
f32 f32_neg_inf(void);
f64 f64_inf(void);
f64 f64_neg_inf(void);

f32 f32_abs(f32 x);
f64 f64_abs(f64 x);

#endif /* STC_DISABLE_BASIC_TYPES */

#endif /* STC_COMMON_H */
