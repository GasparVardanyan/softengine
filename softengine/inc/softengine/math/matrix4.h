# ifndef __SOFTENGINE_MATH_MATRIX4_H
# define __SOFTENGINE_MATH_MATRIX4_H

# include "../math.h"

struct matrix4
{
	scalar_t
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	;
};

extern const struct matrix4 MATRIX4_IDENTITY;
extern const struct matrix4 MATRIX4_ZERO;

struct matrix4 MATRIX4_TRANSLATION (scalar_t x, scalar_t y, scalar_t z);
struct matrix4 MATRIX4_ROTATIONX (scalar_t a);
struct matrix4 MATRIX4_ROTATIONY (scalar_t a);
struct matrix4 MATRIX4_ROTATIONZ (scalar_t a);

struct matrix4 matrix4_mul (struct matrix4 m1, struct matrix4 m2);
struct matrix4 matrix4_translate (struct matrix4 m, scalar_t x, scalar_t y, scalar_t z);

# endif // __SOFTENGINE_MATH_MATRIX4_H
