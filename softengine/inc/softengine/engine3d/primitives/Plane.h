# ifndef __SOFTENGINE3D_PRIMITIVES_PLANE_H
# define __SOFTENGINE3D_PRIMITIVES_PLANE_H

# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

class Plane : public Mesh
{
public:
	Plane (scalar_t width = 1, scalar_t height = 1, bool twosided = true, matrix4 transform = {0});
};

# endif // __SOFTENGINE3D_PRIMITIVES_PLANE_H
