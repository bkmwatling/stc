#ifndef STC_FLOAT_H
#define STC_FLOAT_H

/* NOTE: this unit only works when basic types are enabled */
#ifndef STC_DISABLE_BASIC_TYPES
#    include "../common.h"

#    define F32_PI                 3.14159265359f
#    define F32_TAU                6.28318530718f
#    define F32_E                  2.71828182846f
#    define F32_GOLDEN_RATIO_SMALL 0.61803398875f
#    define F32_GOLDEN_RATIO_BIG   1.61803398875f

#    define F64_PI                 3.14159265359
#    define F64_TAU                6.28318530718
#    define F64_E                  2.71828182846
#    define F64_GOLDEN_RATIO_SMALL 0.61803398875
#    define F64_GOLDEN_RATIO_BIG   1.61803398875

/*** Math macros and functions ************************************************/

#    define lerp(x, a, b)   ((a) + ((b) - (a)) * (x))
#    define unlerp(t, a, b) ((a) != (b) ? ((t) - (a)) / ((b) - (a)) : 0)

f32 f32_abs(f32 x);
f64 f64_abs(f64 x);

f32 f32_sqrt(f32 x);
f32 f32_sin(f32 x);
f32 f32_cos(f32 x);
f32 f32_tan(f32 x);
f32 f32_log(f32 x);

f64 f64_sqrt(f64 x);
f64 f64_sin(f64 x);
f64 f64_cos(f64 x);
f64 f64_tan(f64 x);
f64 f64_log(f64 x);

#endif /* STC_DISABLE_BASIC_TYPES */
#endif /* STC_FLOAT_H */
