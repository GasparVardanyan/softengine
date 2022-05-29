# ifndef __SOFTENGINE_CORE_GEOMETRY_H
# define __SOFTENGINE_CORE_GEOMETRY_H

# include <vector>

# include "softengine/math.h"

class Geometry
{
public:
	std::vector <vertex> vertices;
	std::vector <face> faces; // TODO: don't use scalar_t (vector3) there
};

# endif // __SOFTENGINE_CORE_GEOMETRY_H
