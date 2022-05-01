# ifndef __SOFTENGINE_MATH_MATRIX4_H
# define __SOFTENGINE_MATH_MATRIX4_H

# include "../math.h"

struct vector3;

// typedef scalar_t matrix4 [16]; - don't do this way to be able to easily copy matrices.

struct matrix4
{
	scalar_t
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33; // TODO: do we actually need this row to work with *transform* matrices?
};

static const struct matrix4 MATRIX4_IDENTITY = {
	.m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1
};
static const struct matrix4 MATRIX4_ZERO;
struct matrix4 MATRIX4_ROTATIONX (const scalar_t);
struct matrix4 MATRIX4_ROTATIONY (const scalar_t);
struct matrix4 MATRIX4_ROTATIONZ (const scalar_t);
struct matrix4 MATRIX4_TRANSLATION (const scalar_t, const scalar_t, const scalar_t);

// assignment
struct matrix4 * matrix4_Mul (struct matrix4 * const restrict, const struct matrix4);
struct matrix4 * matrix4_Translate (struct matrix4 * const restrict, const scalar_t, const scalar_t, const scalar_t);

struct matrix4 matrix4_mul (const struct matrix4, const struct matrix4);
struct matrix4 matrix4_translate (struct matrix4, const scalar_t, const scalar_t, const scalar_t);

# endif // __SOFTENGINE_MATH_MATRIX4_H
