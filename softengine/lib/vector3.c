# include "softengine/math/vector3.h"

const struct vector3 VECTOR3_X = { .x = 1 };
const struct vector3 VECTOR3_Y = { .y = 1 };
const struct vector3 VECTOR3_Z = { .z = 1 };
const struct vector3 VECTOR3_ZERO = { 0 };

struct vector3 vector3_transform (struct vector3 v, struct matrix4 m)
{
	scalar_t w = v.x * m._x + v.y * m._y + v.z * m._z + m._w;
	return (struct vector3) {
		(v.x * m.xx + v.y * m.yx + v.z * m.zx + m.tx) / w,
		(v.x * m.xy + v.y * m.yy + v.z * m.zy + m.ty) / w,
		(v.x * m.xz + v.y * m.yz + v.z * m.zz + m.tz) / w
	};
}

struct vector3 vector3_scale (struct vector3 v, scalar_t s)
{
	return (struct vector3) {
		v.x / s, v.y / s, v.z / s
	};
}
