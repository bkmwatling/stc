#include "math.h"

#ifndef STC_DISABLE_BASIC_TYPES
#    ifndef STC_DISABLE_STD_MATH_H
#        include <math.h>

f32 f32_sqrt(f32 x) { return sqrtf(x); }

f32 f32_sin(f32 x) { return sinf(x); }

f32 f32_cos(f32 x) { return cosf(x); }

f32 f32_tan(f32 x) { return tanf(x); }

f32 f32_log(f32 x) { return logf(x); }

f64 f64_sqrt(f64 x) { return sqrt(x); }

f64 f64_sin(f64 x) { return sin(x); }

f64 f64_cos(f64 x) { return cos(x); }

f64 f64_tan(f64 x) { return tan(x); }

f64 f64_log(f64 x) { return log(x); }

#    else

f32 f32_sqrt(f32 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f32_sqrt");
}

f32 f32_sin(f32 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f32_sin");
}

f32 f32_cos(f32 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f32_cos");
}

f32 f32_tan(f32 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f32_tan");
}

f32 f32_log(f32 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f32_log");
}

f64 f64_sqrt(f64 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f64_sqrt");
}

f64 f64_sin(f64 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f64_sin");
}

f64 f64_cos(f64 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f64_cos");
}

f64 f64_tan(f64 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f64_tan");
}

f64 f64_log(f64 x)
{
    STC_UNUSED(x);
    STC_UNIMPLEMENTED_MSG("f64_log");
}

#    endif /* STC_DISABLE_STD_MATH_H */
#endif     /* STC_DISABLE_BASIC_TYPES */
