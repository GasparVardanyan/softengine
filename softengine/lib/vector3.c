# include "softengine/math/vector3.h"

const struct vector3 VECTOR3_X = { .x = 1 };
const struct vector3 VECTOR3_Y = { .y = 1 };
const struct vector3 VECTOR3_Z = { .z = 1 };
const struct vector3 VECTOR3_ZERO = { 0 };

struct vector3 vector3_transform (struct vector3 v, struct matrix4 m)
{
	return (struct vector3) {
		v.x * m.m00 + v.y * m.m01 + v.z * m.m02 + m.m30,
		v.x * m.m10 + v.y * m.m11 + v.z * m.m12 + m.m31,
		v.x * m.m20 + v.y * m.m21 + v.z * m.m22 + m.m32
	};
}

struct vector3 vector3_scale (struct vector3 v, scalar_t s)
{
	return (struct vector3) {
		v.x / s, v.y / s, v.z / s
	};
}
