#include "common.h"

typedef union {
    f32 f;
    u32 u;
} __stc_f32_u32;

typedef union {
    f64 f;
    u64 u;
} __stc_f64_u64;

/* --- Floating point standard and constant functions ----------------------- */

f32 f32_inf(void)
{
    __stc_f32_u32 inf;
    inf.u = 0x7f800000;
    return inf.f;
}

f32 f32_neg_inf(void)
{
    __stc_f32_u32 neg_inf;
    neg_inf.u = 0xff800000;
    return neg_inf.f;
}

f64 f64_inf(void)
{
    __stc_f64_u64 inf;
    inf.u = 0x7ff0000000000000;
    return inf.f;
}

f64 f64_neg_inf(void)
{
    __stc_f64_u64 neg_inf;
    neg_inf.u = 0xfff0000000000000;
    return neg_inf.f;
}
