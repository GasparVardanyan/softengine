# include <math.h>

# include "softengine/math/matrix4.h"

const struct matrix4 MATRIX4_IDENTITY = {
	.m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1
};

const struct matrix4 MATRIX4_ZERO = { 0 };

struct matrix4 MATRIX4_TRANSLATION (scalar_t x, scalar_t y, scalar_t z)
{
	return (struct matrix4) {
		.m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1,
		.m30 = x, .m31 = y, .m32 = z
	};
}

struct matrix4 MATRIX4_ROTATIONX (scalar_t a)
{
	return (struct matrix4) {
		1, 0, 0, 0,
		0, cos (a), -sin (a), 0,
		0, sin (a), cos (a), 0,
		0, 0, 0, 1
	};
}

struct matrix4 MATRIX4_ROTATIONY (scalar_t a)
{
	return (struct matrix4) {
		cos (a), 0, sin (a), 0,
		0, 1, 0, 0,
		-sin (a), 0, cos (a), 0,
		0, 0, 0, 1
	};
}

struct matrix4 MATRIX4_ROTATIONZ (scalar_t a)
{
	return (struct matrix4) {
		cos (a), -sin (a), 0, 0,
		sin (a), cos (a), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

struct matrix4 matrix4_mul (struct matrix4 m1, struct matrix4 m2)
{
	/* struct matrix4 m; */
	/* scalar_t * m1_raw = (scalar_t *) & m1; */
	/* scalar_t * m2_raw = (scalar_t *) & m2; */
	/* scalar_t * m_raw = (scalar_t *) & m; */
    /*  */
	/* for (int i = 0; i < 4; i++) */
	/*     for (int j = 0; j < 4; j++) */
	/*     { */
	/*         scalar_t s = 0; */
	/*         for (int k = 0; k < 4; k++) */
	/*             s += m1_raw [i * 4 + k] * m2_raw [k * 4 + j]; */
	/*         m_raw [i * 4 + j] = s; */
	/*     } */
    /*  */
	/* return m; */

	return (struct matrix4) {
		.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30,
		.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31,
		.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32,
		.m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33,
		.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30,
		.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31,
		.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32,
		.m13 = m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33,
		.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30,
		.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31,
		.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32,
		.m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33,
		.m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30,
		.m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31,
		.m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32,
		.m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33
	};

}

struct matrix4 matrix4_translate (struct matrix4 m, scalar_t x, scalar_t y, scalar_t z)
{
	m.m03 = x;
	m.m13 = y;
	m.m23 = z;

	return m;
}
