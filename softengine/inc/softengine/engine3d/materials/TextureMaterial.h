# ifndef __SOFTENGINE3D_MATERIALS_TEXTUREMATERIAL_H
# define __SOFTENGINE3D_MATERIALS_TEXTUREMATERIAL_H

# include "softengine/engine3d/core/Image.h"
# include "softengine/engine3d/core/Material.h"

class TextureMaterial : public Material
{
public:
	Image raw;

	TextureMaterial ()
	{
	}

	virtual color4 map (scalar_t u, scalar_t v) const
	{
		u *= raw.width;
		v *= raw.height;

		int x1 = u;
		int y1 = v;
		int x2 = u + 1;
		int y2 = v + 1;

		u -= x1;
		v -= y1;

		color4 tl = raw.get_pixel (x1, y1);
		color4 tr = raw.get_pixel (x2, y1);
		color4 bl = raw.get_pixel (x1, y2);
		color4 br = raw.get_pixel (x2, y2);

		// TODO: do linear filtering there...
		return tl;
	}
};

# endif // __SOFTENGINE3D_MATERIALS_TEXTUREMATERIAL_H
