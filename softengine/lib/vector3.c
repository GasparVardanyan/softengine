# include <math.h>

# include "softengine/math.h"

const struct vector3 VECTOR3_X = { .x = 1 };
const struct vector3 VECTOR3_Y = { .y = 1 };
const struct vector3 VECTOR3_Z = { .z = 1 };
const struct vector3 VECTOR3_ZERO = { 0 };

const struct vector3 VECTOR3_UP = { .z = 1 };
const struct vector3 VECTOR3_DOWN = { .z = -1 };
const struct vector3 VECTOR3_LEFT = { .x = -1 };
const struct vector3 VECTOR3_RIGHT = { .x = 1 };
const struct vector3 VECTOR3_FORWARD = { .z = 1 };
const struct vector3 VECTOR3_BACK = { .z = -1 };

struct vector3 vector3_transform (struct vector3 v, struct matrix4 m)
{
	scalar_t w = v.x * m.xw + v.y * m.yw + v.z * m.zw + m.tw;
	return (struct vector3) {
		(v.x * m.xx + v.y * m.yx + v.z * m.zx + m.tx) / w,
		(v.x * m.xy + v.y * m.yy + v.z * m.zy + m.ty) / w,
		(v.x * m.xz + v.y * m.yz + v.z * m.zz + m.tz) / w
	};
}

struct vector3 vector3_scale (struct vector3 v, scalar_t s)
{
	return (struct vector3) {
		v.x * s, v.y * s, v.z * s
	};
}

struct vector3 vector3_negative (struct vector3 v)
{
	return (struct vector3) {-v.x, -v.y, -v.z};
}

scalar_t vector3_length (struct vector3 v)
{
	return sqrtl (v.x * v.x + v.y * v.y + v.z * v.z);
}

scalar_t vector3_lengthsqr (struct vector3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

// TODO: improove performance of vector operations

struct vector3 vector3_normalized (struct vector3 v)
{
	scalar_t l = vector3_length (v);
	return (struct vector3) {
		v.x / l,
		v.y / l,
		v.z / l
	};
}

scalar_t vector3_dot (struct vector3 v1, struct vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct vector3 vector3_cross (struct vector3 v1, struct vector3 v2)
{
	return (struct vector3) {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

scalar_t vector3_angle (struct vector3 v1, struct vector3 v2)
{
	return acos (vector3_dot (v1, v2) / vector3_length (v1) / vector3_length (v2));
}

scalar_t vector3_anglecos (struct vector3 v1, struct vector3 v2)
{
	return vector3_dot (v1, v2) / vector3_length (v1) / vector3_length (v2);
}

scalar_t vector3_anglesin (struct vector3 v1, struct vector3 v2)
{
	return vector3_length (vector3_cross (v1, v2)) / vector3_length (v1) / vector3_length (v2);
}

scalar_t vector3_distsqr (struct vector3 v1, struct vector3 v2)
{
	scalar_t xdist = v1.x - v2.x;
	scalar_t ydist = v1.y - v2.y;
	scalar_t zdist = v1.z - v2.z;

	return xdist * xdist + ydist * ydist + zdist * zdist;
}

scalar_t vector3_dist (struct vector3 v1, struct vector3 v2)
{
	return sqrtl (vector3_distsqr (v1, v2));
}

struct vector3 vector3_add (struct vector3 v1, struct vector3 v2)
{
	return (struct vector3) {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
}

struct vector3 vector3_sub (struct vector3 v1, struct vector3 v2)
{
	return (struct vector3) {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	};
}
