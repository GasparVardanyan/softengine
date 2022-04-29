# ifndef __SOFTENGINE_MATH_VECTOR3_H
# define __SOFTENGINE_MATH_VECTOR3_H

# include "../math.h"
# include "matrix4.h"

struct matrix4;

struct vector3
{
	scalar_t x;
	scalar_t y;
	scalar_t z;
};

static const struct vector3 VECTOR3_X = { .x = 1 };
static const struct vector3 VECTOR3_Y = { .y = 1 };
static const struct vector3 VECTOR3_Z = { .z = 1 };
static const struct vector3 VECTOR3_ZERO;

// assignment
struct vector3 * vector3_Add (struct vector3 * const restrict, const struct vector3);
struct vector3 * vector3_Sub (struct vector3 * const restrict, const struct vector3);
struct vector3 * vector3_Mul (struct vector3 * const restrict, const scalar_t);
struct vector3 * vector3_Norm (struct vector3 * const restrict);
struct vector3 * vector3_Transform (struct vector3 * const restrict, const struct matrix4);

struct vector3 vector3_add (const struct vector3, const struct vector3);
struct vector3 vector3_sub (const struct vector3, const struct vector3);
struct vector3 vector3_mul (const struct vector3, const scalar_t);
struct vector3 vector3_norm (const struct vector3);
scalar_t vector3_len (const struct vector3);
scalar_t vector3_dot (const struct vector3, const struct vector3);
struct vector3 vector3_cross (const struct vector3, const struct vector3);
struct vector3 vector3_transform (const struct vector3, const struct matrix4);

# endif // __SOFTENGINE_MATH_VECTOR3_H
