# ifndef __SOFTENGINE3D_PRIMITIVES_BOX_H
# define __SOFTENGINE3D_PRIMITIVES_BOX_H

# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

class Box : public Mesh
{
public:
	Box (scalar_t width = 1, scalar_t height = 1, scalar_t length = 1, matrix4 transform = {0});
};

# endif // __SOFTENGINE3D_PRIMITIVES_BOX_H
