# ifndef __SOFTENGINE3D_OBJECTS_MESH_H
# define __SOFTENGINE3D_OBJECTS_MESH_H

# include <memory>

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Material.h"
# include "softengine/engine3d/core/Object3D.h"

class Mesh : public Object3D
{
	friend class Scene;
	friend class Camera3D;
	friend class ParserBABYLON;

protected:
	Geometry * geometry;
	std::shared_ptr <Material> material;

public:
	Mesh ()
		: geometry (nullptr)
		, material (nullptr)
	{}

	virtual ~Mesh ()
	{
		if (this->geometry)
			delete this->geometry;
	}

	void setMaterial (Material * material)
	{
		this->material = std::shared_ptr <Material> (material);
	}
};

# endif // __SOFTENGINE3D_OBJECTS_MESH_H
