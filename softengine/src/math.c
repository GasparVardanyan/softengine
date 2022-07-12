# include <math.h>

# include "softengine/math.h"

# define M_PI 3.141592653589793L

scalar_t clamp (scalar_t value, scalar_t min, scalar_t max)
{
	if (value < min) value = min;
	if (value > max) value = max;

	return value;
}

scalar_t interpolate (scalar_t min, scalar_t max, scalar_t gradient)
{
	return min + (max - min) * clamp (gradient, 0, 1);
}

scalar_t fov_to_efl (scalar_t fov, unsigned image_plane_size)
{
	return 1 / tan (fov / 2) * image_plane_size / 2.l;
}

scalar_t efl_to_fov (scalar_t efl, unsigned image_plane_size)
{
	return 2 * (M_PI / 2 - atan (2 * efl / image_plane_size));
}
