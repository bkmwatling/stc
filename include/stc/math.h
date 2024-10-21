#ifndef STC_MATH_H
#define STC_MATH_H

typedef float  f32;
typedef double f64;

#define F32_PI                 3.14159265359f
#define F32_TAU                6.28318530718f
#define F32_E                  2.71828182846f
#define F32_GOLDEN_RATIO_SMALL 0.61803398875f
#define F32_GOLDEN_RATIO_BIG   1.61803398875f

#define F64_PI                 3.14159265359
#define F64_TAU                6.28318530718
#define F64_E                  2.71828182846
#define F64_GOLDEN_RATIO_SMALL 0.61803398875
#define F64_GOLDEN_RATIO_BIG   1.61803398875

/* --- Math macros and functions -------------------------------------------- */

#define LERP(x, a, b)   ((a) + ((b) - (a)) * (x))
#define UNLERP(t, a, b) ((a) != (b) ? ((t) - (a)) / ((b) - (a)) : 0)

/**
 * Calculate the absolute value of a 32-bit floating point number.
 *
 * If x is a NaN, a NaN is returned.
 * If x is -0, +0 is returned.
 * If x is negative or positive infinity, positive infinity is returned.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the absolute value of the 32-bit floating point number
 */
f32 f32_abs(f32 x);

/**
 * Calculate the square root value of a 32-bit floating point number.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the square root value of the 32-bit floating point number
 */
f32 f32_sqrt(f32 x);

/**
 * Calculate the sine value of a 32-bit floating point number.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the sine value of the 32-bit floating point number
 */
f32 f32_sin(f32 x);

/**
 * Calculate the cosine value of a 32-bit floating point number.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the cosine value of the 32-bit floating point number
 */
f32 f32_cos(f32 x);

/**
 * Calculate the tangent value of a 32-bit floating point number.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the tangent value of the 32-bit floating point number
 */
f32 f32_tan(f32 x);

/**
 * Calculate the natural logarithm value of a 32-bit floating point number.
 *
 * @param[in] x the 32-bit floating point number
 *
 * @return the natural logarithm value of the 32-bit floating point number
 */
f32 f32_log(f32 x);

/**
 * Calculate the absolute value of a 64-bit floating point number.
 *
 * If x is a NaN, a NaN is returned.
 * If x is -0, +0 is returned.
 * If x is negative or positive infinity, positive infinity is returned.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the absolute value of the 64-bit floating point number
 */
f64 f64_abs(f64 x);

/**
 * Calculate the square root value of a 64-bit floating point number.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the square root value of the 64-bit floating point number
 */
f64 f64_sqrt(f64 x);

/**
 * Calculate the sine value of a 64-bit floating point number.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the sine value of the 64-bit floating point number
 */
f64 f64_sin(f64 x);

/**
 * Calculate the cosine value of a 64-bit floating point number.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the cosine value of the 64-bit floating point number
 */
f64 f64_cos(f64 x);

/**
 * Calculate the tangent value of a 64-bit floating point number.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the tangent value of the 64-bit floating point number
 */
f64 f64_tan(f64 x);

/**
 * Calculate the natural logarithm value of a 64-bit floating point number.
 *
 * @param[in] x the 64-bit floating point number
 *
 * @return the natural logarithm value of the 64-bit floating point number
 */
f64 f64_log(f64 x);

#endif /* STC_MATH_H */
