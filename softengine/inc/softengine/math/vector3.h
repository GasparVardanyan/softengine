# ifndef __SOFTENGINE_MATH_VECTOR3_H
# define __SOFTENGINE_MATH_VECTOR3_H

# include "../math.h"

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

# endif // __SOFTENGINE_MATH_VECTOR3_H
