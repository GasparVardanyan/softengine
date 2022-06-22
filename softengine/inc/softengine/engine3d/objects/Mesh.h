# ifndef __SOFTENGINE_OBJECTS_MESH_H
# define __SOFTENGINE_OBJECTS_MESH_H

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"

class Mesh : public Object3D
{
	friend class Scene;
	friend class Camera3D;
	friend class ParserBABYLON;

protected:
	Geometry * geometry;

public:
	Mesh () : geometry (nullptr) {}

	virtual ~Mesh ()
	{
		if (this->geometry)
			delete this->geometry;
	}
};

# endif // __SOFTENGINE_OBJECTS_MESH_H
