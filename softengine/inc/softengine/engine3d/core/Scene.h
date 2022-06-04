# ifndef __SOFTENGINE_CORE_SCENE_H
# define __SOFTENGINE_CORE_SCENE_H

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

class Scene : public Mesh
{
	friend class Camera3D;

protected:
	// Geometry geometry;
	std::size_t _num_vertices;
	std::size_t _num_faces;
	void update (const Object3D * container, matrix4 transform = MATRIX4_IDENTITY);

public:
	const Object3D * root_container;

	Scene (const Object3D * root_container) :
		root_container (root_container),
		_num_vertices (0),
		_num_faces (0)
	{
	}

	void update ();
};

# endif // __SOFTENGINE_CORE_SCENE_H
