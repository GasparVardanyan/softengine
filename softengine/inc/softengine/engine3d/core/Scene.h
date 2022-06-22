# ifndef __SOFTENGINE_CORE_SCENE_H
# define __SOFTENGINE_CORE_SCENE_H

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/lights/PointLight.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

class Scene// : public Mesh
{
	friend class Camera3D;

protected:
	Geometry * geometry;
	PointLight ** point_lights;

	std::size_t _num_vertices;
	std::size_t _num_faces;
	std::size_t _i_vertices;
	std::size_t _i_faces;
	std::size_t _num_point_lights;
	std::size_t _i_point_lights;

	void update (Object3D * container, matrix4 transform = MATRIX4_IDENTITY);
	void prepare (const Object3D * container);

public:
	Object3D * root_container;

	Scene (Object3D * root_container) :
		root_container (root_container),
		_num_vertices (0),
		_num_faces (0),
		_i_vertices (0),
		_i_faces (0),
		_num_point_lights (0),
		_i_point_lights (0),
		geometry (nullptr),
		point_lights (nullptr)
	{
	}

	void update ();

	virtual ~Scene ()
	{
		if (this->geometry)
			delete this->geometry;
		if (this->point_lights)
			delete this->point_lights;
	}
};

# endif // __SOFTENGINE_CORE_SCENE_H
