# ifndef __SOFTENGINE3D_CORE_MATERIAL_H
# define __SOFTENGINE3D_CORE_MATERIAL_H

# include "softengine/math.h"

class Material
{
	public:
		bool lights;

		Material (bool lights) : lights (lights) {}

		virtual color4 map (scalar_t u, scalar_t v) const { return (color4){}; }
};

# endif // __SOFTENGINE3D_CORE_MATERIAL_H
