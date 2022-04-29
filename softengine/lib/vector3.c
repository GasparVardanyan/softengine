# include <math.h> // sqrt

# include "math.h"
# include "softengine/math/vector3.h"

struct vector3 * vector3_Add (struct vector3 * const restrict v1, const struct vector3 v2)
{
	v1 -> x += v2.x;
	v1 -> y += v2.y;
	v1 -> z += v2.z;

	return v1;
}

struct vector3 * vector3_Sub (struct vector3 * const restrict v1, const struct vector3 v2)
{
	v1 -> x -= v2.x;
	v1 -> y -= v2.y;
	v1 -> z -= v2.z;

	return v1;
}

struct vector3 * vector3_Mul (struct vector3 * const restrict v, const scalar_t s)
{
	v -> x *= s;
	v -> y *= s;
	v -> z *= s;

	return v;
}

struct vector3 * vector3_Norm (struct vector3 * const restrict v)
{
	const scalar_t l = vector3_len (* v);

	v -> x /= l;
	v -> y /= l;
	v -> z /= l;

	return v;
}

struct vector3 * vector3_Transform (struct vector3 * const restrict v, const struct matrix4 m)
{
	const scalar_t w = v -> x * m.m30 + v -> y * m.m31 + v -> z * m.m32 + 1 * m.m33;

	// TODO: do we actually need *w* to work with *transform* matrices?

	v -> x = (v -> x * m.m00 + v -> y * m.m01 + v -> z * m.m02 + m.m03) / w;
	v -> y = (v -> x * m.m10 + v -> y * m.m11 + v -> z * m.m12 + m.m13) / w;
	v -> z = (v -> x * m.m20 + v -> y * m.m21 + v -> z * m.m22 + m.m23) / w;

	return v;
}

struct vector3 vector3_add (const struct vector3 v1, const struct vector3 v2)
{
	return (struct vector3) { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
}

struct vector3 vector3_sub (const struct vector3 v1, const struct vector3 v2)
{
	return (struct vector3) { .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z };
}

struct vector3 vector3_mul (const struct vector3 v, const scalar_t s)
{
	return (struct vector3) { .x = v.x  * s, .y = v.y  * s, .z = v.z  * s };
}

struct vector3 vector3_norm (const struct vector3 v)
{
	const scalar_t l = vector3_len (v);

	return (struct vector3) { .x = v.x / l, .y = v.y / l, .z = v.z / l};
}

scalar_t vector3_len (const struct vector3 v)
{
	return sqrt (v.x * v.x + v.y * v.y + v.z * v.z);
}

scalar_t vector3_dot (const struct vector3 v1, const struct vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct vector3 vector3_cross (const struct vector3 v1, const struct vector3 v2)
{
	return (struct vector3) {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.z
	};
}

struct vector3 vector3_transform (const struct vector3 v, const struct matrix4 m)
{
	const scalar_t w = v.x * m.m30 + v.y * m.m31 + v.z * m.m32 + 1 * m.m33;

	// TODO: do we actually need *w* to work with *transform* matrices?

	return (struct vector3) {
		.x = (v.x * m.m00 + v.y * m.m01 + v.z * m.m02 + m.m03) / w,
		.y = (v.x * m.m10 + v.y * m.m11 + v.z * m.m12 + m.m13) / w,
		.z = (v.x * m.m20 + v.y * m.m21 + v.z * m.m22 + m.m23) / w
	};
}
