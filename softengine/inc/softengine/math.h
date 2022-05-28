# ifndef __SOFTENGINE_MATH_H
# define __SOFTENGINE_MATH_H

# ifdef __cplusplus
extern "C" {
# endif // __cplusplus

typedef long double scalar_t;

struct vector3;
struct matrix4;

typedef struct vector3 vector3;
typedef struct matrix4 matrix4;



struct vector3
{
	scalar_t x, y, z;
};

extern const struct vector3 VECTOR3_X;
extern const struct vector3 VECTOR3_Y;
extern const struct vector3 VECTOR3_Z;
extern const struct vector3 VECTOR3_ZERO;

struct vector3 vector3_transform (struct vector3 v, struct matrix4 m);
struct vector3 vector3_scale (struct vector3 v, scalar_t s);



struct matrix4
{
	union
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
			vector3 X; scalar_t _x;
			vector3 Y; scalar_t _y;
			vector3 Z; scalar_t _z;
			vector3 T; scalar_t _t;
		};
	};
};

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

struct matrix4 perspective_projection (scalar_t fov, scalar_t znear, scalar_t zfar, scalar_t aspect_ratio);

_Bool matrix4_equals (matrix4 a, matrix4 b);



# ifdef __cplusplus
}
# endif // __cplusplus

# endif // __SOFTENGINE_MATH_H
