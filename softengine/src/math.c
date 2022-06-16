# include "softengine/math.h"

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
