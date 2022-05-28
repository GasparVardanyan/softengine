# ifndef __SOFTENGINE_OBJECTS_MESH_H
# define __SOFTENGINE_OBJECTS_MESH_H

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"

class Mesh : public Object3D
{
public:
	Geometry geometry;
};

# endif // __SOFTENGINE_OBJECTS_MESH_H
