# include <math.h>

# include "softengine/math/matrix4.h"

const struct matrix4 MATRIX4_IDENTITY = {
	.m00 = 1, .m11 = 1, .m22 = 1, .m33 = 1
};

const struct matrix4 MATRIX4_ZERO = { 0 };

struct matrix4 MATRIX4_TRANSLATION (scalar_t x, scalar_t y, scalar_t z)
{
	return (struct matrix4) {
		.xx = 1, .yy = 1, .zz = 1, ._w = 1,
		.tx = x, .ty = y, .tz = z
	};
}

struct matrix4 MATRIX4_ROTATIONX (scalar_t a)
{
	scalar_t c = cos (a), s = sin (a);
	return (struct matrix4) {
		 1,  0,  0,  0,
		 0,  c,  s,  0,
		 0, -s,  c,  0,
		 0,  0,  0,  1,
	};
	/* return (struct matrix4) { */
	/*     .xx = 1, ._w = 1, */
	/*     .yy = c, .zy = -s, */
	/*     .yz = s, .zz = c */
	/* }; */
}

struct matrix4 MATRIX4_ROTATIONY (scalar_t a)
{
	scalar_t c = cos (a), s = sin (a);
	return (struct matrix4) {
		 c,  0, -s,  0,
		 0,  1,  0,  0,
		 s,  0,  c,  0,
		 0,  0,  0,  1,
	};
	/* return (struct matrix4) { */
	/*     .yy = 1, ._w = 1, */
	/*     .xx = c, .zx = s, */
	/*     .xz = -s, .zz = c */
	/* }; */
}

struct matrix4 MATRIX4_ROTATIONZ (scalar_t a)
{
	scalar_t c = cos (a), s = sin (a);
	return (struct matrix4) {
		 c,  s,  0,  0,
		-s,  c,  0,  0,
		 0,  0,  1,  0,
		 0,  0,  0,  1,
	};
	/* return (struct matrix4) { */
	/*     .yy = 1, ._w = 1, */
	/*     .xx = c, .zx = s, */
	/*     .xz = -s, .zz = c */
	/* }; */
}

struct matrix4 matrix4_mul (struct matrix4 m1, struct matrix4 m2)
{
	/* struct matrix4 m; */
    /*  */
	/* for (int i = 0; i < 4; i++) */
	/*     for (int j = 0; j < 4; j++) */
	/*     { */
	/*         scalar_t s = 0; */
	/*         for (int k = 0; k < 4; k++) */
	/*             s += m1.m [i * 4 + k] * m2.m [k * 4 + j]; */
	/*         m.m [i * 4 + j] = s; */
	/*     } */
    /*  */
	/* return m; */

	return (struct matrix4) {
		m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30,
		m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31,
		m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32,
		m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33,
		m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30,
		m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31,
		m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32,
		m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33,
		m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30,
		m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31,
		m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32,
		m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33,
		m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30,
		m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31,
		m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32,
		m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33
	};
}

struct matrix4 matrix4_translate (struct matrix4 m, scalar_t x, scalar_t y, scalar_t z)
{
	m.tx = x;
	m.ty = y;
	m.tz = z;

	return m;
}
