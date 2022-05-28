# ifndef __SOFTENGINE_CORE_GEOMETRY_H
# define __SOFTENGINE_CORE_GEOMETRY_H

# include <vector>

# include "softengine/math.h"

class Geometry
{
public:
	std::vector <vector3> vertices;
	std::vector <vector3> faces;
};

# endif // __SOFTENGINE_CORE_GEOMETRY_H
