# ifndef __SOFTENGINE3D_MATERIALS_FILLMATERIAL_H
# define __SOFTENGINE3D_MATERIALS_FILLMATERIAL_H

# include "softengine/engine3d/core/Image.h"
# include "softengine/engine3d/core/Material.h"

class FillMaterial : public Material
{
public:
	color4 color;

	FillMaterial (color4 color, bool lights = true)
		: Material (lights)
		, color (color)
	{
	}

	virtual color4 map (scalar_t u, scalar_t v) const
	{
		return color;
	}
};

# endif // __SOFTENGINE3D_MATERIALS_FILLMATERIAL_H
