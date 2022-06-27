# ifndef __SOFTENGINE_MATH_H
# define __SOFTENGINE_MATH_H

# ifdef __cplusplus
extern "C" {
# endif // __cplusplus

typedef long double scalar_t;

struct vector3;
struct matrix4;
struct color4;

typedef struct vector3 vector3;
typedef struct matrix4 matrix4;
typedef struct color4 color4;
typedef struct point point;

scalar_t clamp (scalar_t value, scalar_t min, scalar_t max);
scalar_t interpolate (scalar_t min, scalar_t max, scalar_t gradient);



struct color4
{
	unsigned char r, g, b;
	scalar_t a;
};

struct point
{
	int x, y;
	scalar_t _z;
};



struct vector3
{
	scalar_t x, y, z;
};

extern const struct vector3 VECTOR3_X;
extern const struct vector3 VECTOR3_Y;
extern const struct vector3 VECTOR3_Z;
extern const struct vector3 VECTOR3_ZERO;
extern const struct vector3 VECTOR3_UP;
extern const struct vector3 VECTOR3_DOWN;
extern const struct vector3 VECTOR3_LEFT;
extern const struct vector3 VECTOR3_RIGHT;
extern const struct vector3 VECTOR3_FORWARD;
extern const struct vector3 VECTOR3_BACK;

struct vector3 vector3_transform (struct vector3 v, struct matrix4 m);
struct vector3 vector3_transform_normal (struct vector3 v, struct matrix4 m);
struct vector3 vector3_scale (struct vector3 v, scalar_t s);
struct vector3 vector3_negative (struct vector3 v);
scalar_t vector3_length (struct vector3 v);
scalar_t vector3_lengthsqr (struct vector3 v);
struct vector3 vector3_normalized (struct vector3 v);
scalar_t vector3_dot (struct vector3 v1, struct vector3 v2);
struct vector3 vector3_cross (struct vector3 v1, struct vector3 v2);
scalar_t vector3_angle (struct vector3 v1, struct vector3 v2);
scalar_t vector3_cos (struct vector3 v1, struct vector3 v2);
scalar_t vector3_sin (struct vector3 v1, struct vector3 v2);
scalar_t vector3_distsqr (struct vector3 v1, struct vector3 v2);
scalar_t vector3_dist (struct vector3 v1, struct vector3 v2);
struct vector3 vector3_add (struct vector3 v1, struct vector3 v2);
struct vector3 vector3_sub (struct vector3 v1, struct vector3 v2);



struct matrix4 { union
{
	struct
	{
		scalar_t
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		;
	};
	scalar_t m [16];

	struct
	{
		scalar_t
			xx, xy, xz, xw,
			yx, yy, yz, yw,
			zx, zy, zz, zw,
			tx, ty, tz, tw
		;
	};
	struct
	{
		struct vector3 X; scalar_t _x;
		struct vector3 Y; scalar_t _y;
		struct vector3 Z; scalar_t _z;
		struct vector3 T; scalar_t _t;
	};
}; };

extern const struct matrix4 MATRIX4_IDENTITY;
extern const struct matrix4 MATRIX4_ZERO;

struct matrix4 MATRIX4_TRANSLATION (scalar_t x, scalar_t y, scalar_t z);
struct matrix4 MATRIX4_ROTATIONX (scalar_t a);
struct matrix4 MATRIX4_ROTATIONY (scalar_t a);
struct matrix4 MATRIX4_ROTATIONZ (scalar_t a);
struct matrix4 MATRIX4_SCALE (scalar_t x, scalar_t y, scalar_t z);
struct matrix4 MATRIX4_TRANSFORM (vector3 pos, vector3 rot, vector3 scl);

struct matrix4 matrix4_mul (struct matrix4 m1, struct matrix4 m2);
struct matrix4 matrix4_translate (struct matrix4 m, scalar_t x, scalar_t y, scalar_t z);

struct matrix4 perspective_projector (scalar_t fov, scalar_t znear, scalar_t zfar, scalar_t aspect_ratio);

_Bool matrix4_equals (matrix4 a, matrix4 b);



# ifdef __cplusplus
}
# endif // __cplusplus

# endif // __SOFTENGINE_MATH_H
