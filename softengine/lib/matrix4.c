# include <math.h>	// sin, cos
# include "softengine/math/matrix4.h"

struct matrix4 * matrix4_Mul (struct matrix4 * const restrict m1, const struct matrix4 m2)
{
	/*
	for mi in range (4) :
		for mj in range (4) :
			print (
				"\t\t.m{:02} = ".format (mi * 10 + mj) +
				" + ".join (["m1 -> m{:02} * m2.m{:02}".format (mi * 10 + i, i * 10 + mj) for i in range (4)]) +
				","
			)
	*/

	* m1 = (struct matrix4) {
		.m00 = m1 -> m00 * m2.m00 + m1 -> m01 * m2.m10 + m1 -> m02 * m2.m20 + m1 -> m03 * m2.m30,
		.m01 = m1 -> m00 * m2.m01 + m1 -> m01 * m2.m11 + m1 -> m02 * m2.m21 + m1 -> m03 * m2.m31,
		.m02 = m1 -> m00 * m2.m02 + m1 -> m01 * m2.m12 + m1 -> m02 * m2.m22 + m1 -> m03 * m2.m32,
		.m03 = m1 -> m00 * m2.m03 + m1 -> m01 * m2.m13 + m1 -> m02 * m2.m23 + m1 -> m03 * m2.m33,
		.m10 = m1 -> m10 * m2.m00 + m1 -> m11 * m2.m10 + m1 -> m12 * m2.m20 + m1 -> m13 * m2.m30,
		.m11 = m1 -> m10 * m2.m01 + m1 -> m11 * m2.m11 + m1 -> m12 * m2.m21 + m1 -> m13 * m2.m31,
		.m12 = m1 -> m10 * m2.m02 + m1 -> m11 * m2.m12 + m1 -> m12 * m2.m22 + m1 -> m13 * m2.m32,
		.m13 = m1 -> m10 * m2.m03 + m1 -> m11 * m2.m13 + m1 -> m12 * m2.m23 + m1 -> m13 * m2.m33,
		.m20 = m1 -> m20 * m2.m00 + m1 -> m21 * m2.m10 + m1 -> m22 * m2.m20 + m1 -> m23 * m2.m30,
		.m21 = m1 -> m20 * m2.m01 + m1 -> m21 * m2.m11 + m1 -> m22 * m2.m21 + m1 -> m23 * m2.m31,
		.m22 = m1 -> m20 * m2.m02 + m1 -> m21 * m2.m12 + m1 -> m22 * m2.m22 + m1 -> m23 * m2.m32,
		.m23 = m1 -> m20 * m2.m03 + m1 -> m21 * m2.m13 + m1 -> m22 * m2.m23 + m1 -> m23 * m2.m33,
		.m30 = m1 -> m30 * m2.m00 + m1 -> m31 * m2.m10 + m1 -> m32 * m2.m20 + m1 -> m33 * m2.m30,
		.m31 = m1 -> m30 * m2.m01 + m1 -> m31 * m2.m11 + m1 -> m32 * m2.m21 + m1 -> m33 * m2.m31,
		.m32 = m1 -> m30 * m2.m02 + m1 -> m31 * m2.m12 + m1 -> m32 * m2.m22 + m1 -> m33 * m2.m32,
		.m33 = m1 -> m30 * m2.m03 + m1 -> m31 * m2.m13 + m1 -> m32 * m2.m23 + m1 -> m33 * m2.m33
	};

	return m1;
}

/* struct matrix4 * matrix4_Rotate (struct matrix4 * const restrict m, scalar_t rotX, scalar_t rotY, scalar_t rotZ) */
/* { */
/*     // how to combine rotations? */
/* } */

struct matrix4 MATRIX4_ROTATIONX (scalar_t a)
{
	const scalar_t cosa = cos (a);
	const scalar_t sina = sin (a);

	return (struct matrix4) {
		.m00 = 1, .m33 = 1,
		.m11 = cosa, .m12 = -sina,
		.m21 = sina, .m22 = cosa
	};
}

struct matrix4 MATRIX4_ROTATIONY (scalar_t a)
{
	const scalar_t cosa = cos (a);
	const scalar_t sina = sin (a);

	return (struct matrix4) {
		.m11 = 1, .m33 = 1,
		.m00 = cosa, .m02 = sina,
		.m20 = -sina, .m22 = cosa
	};
}

struct matrix4 MATRIX4_ROTATIONZ (scalar_t a)
{
	const scalar_t cosa = cos (a);
	const scalar_t sina = sin (a);

	return (struct matrix4) {
		.m22 = 1, .m33 = 1,
		.m00 = cosa, .m01 = -sina,
		.m10 = -sina, .m11 = cosa
	};
}

struct matrix4 matrix4_mul (const struct matrix4 m1, const struct matrix4 m2)
{
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
