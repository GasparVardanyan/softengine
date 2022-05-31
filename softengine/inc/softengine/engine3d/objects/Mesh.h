# ifndef __SOFTENGINE_OBJECTS_MESH_H
# define __SOFTENGINE_OBJECTS_MESH_H

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"

class Mesh : public Object3D
{
	friend class Camera3D;
	friend class ParserBABYLON;

protected:
	Geometry geometry;
};

# endif // __SOFTENGINE_OBJECTS_MESH_H
