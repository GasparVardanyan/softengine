# ifndef __SOFTENGINE_MATH_H
# define __SOFTENGINE_MATH_H

typedef long double scalar_t;
# define PRI_SCALAR_T "LG"

# include "math/vector3.h"
# include "math/matrix4.h"


struct vector3;
struct matrix4;

typedef struct vector3 vector3;
typedef struct matrix4 matrix4;




# if defined (add) ||		\
	defined (sub) ||		\
	defined (mul) ||		\
	defined (norm)
#	error conflicting macro names
# endif

# define add(x, y) _Generic ((x),			\
	struct vector3 : _Generic ((y),			\
		default: vector3_add				\
	),										\
	struct vector3 * : _Generic ((y),		\
		default: vector3_Add				\
	)										\
) (x, y)									\
// add

# define sub(x, y) _Generic ((x),			\
	struct vector3 : _Generic ((y),			\
		default: vector3_sub				\
	),										\
	struct vector3 * : _Generic ((y),		\
		default: vector3_Sub				\
	)										\
) (x, y)									\
// sub

# define mul(x, y) _Generic ((x),			\
	struct vector3 : _Generic ((y),			\
		default: vector3_mul				\
	),										\
	struct vector3 * : _Generic ((y),		\
		default: vector3_Mul				\
	)										\
) (x, y)									\
// mul

# define norm(x) _Generic ((x),				\
	struct vector3 :  vector3_norm,			\
	struct vector3 * : vector3_Norm			\
) (x)										\
// norm

/*
# define len(x) _Generic ((x),				\
	struct vector3 : vector3_len			\
) (x)										\
// norm
*/


# endif // __SOFTENGINE_MATH_H
