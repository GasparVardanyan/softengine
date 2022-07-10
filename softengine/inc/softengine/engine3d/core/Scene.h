# ifndef __SOFTENGINE3D_CORE_SCENE_H
# define __SOFTENGINE3D_CORE_SCENE_H

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Material.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/lights/PointLight.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

class Camera3D;

struct vertex_data
{
	union {
		vector3 projected;
		struct {
			scalar_t x, y, z;
		};
	};
	vector3 world_pos;
	vector3 normal;
	union {
		uv texture_coordinates;
		struct {
			scalar_t u, v;
		};
	};
	union {
		scalar_t intensity;
		scalar_t i;
	};
};

class Scene// : public Mesh
{
	friend class Camera3D;

protected:
	Geometry * geometry;
	const PointLight ** point_lights;
	const Material ** materials;

	std::size_t _i_meshes;
	std::size_t _num_vertices;
	std::size_t _num_faces;
	std::size_t _i_vertices;
	std::size_t _i_faces;
	std::size_t _i_point_lights;

	void update (Object3D * container, matrix4 transform = MATRIX4_IDENTITY);
	void prepare (const Object3D * container);

public:
	Object3D * root_container;
	std::size_t num_point_lights;

	Scene (Object3D * root_container) :
		root_container (root_container),
		_i_meshes (0),
		_num_vertices (0),
		_num_faces (0),
		_i_vertices (0),
		_i_faces (0),
		num_point_lights (0),
		_i_point_lights (0),
		geometry (nullptr),
		point_lights (nullptr),
		materials (nullptr)
	{
	}

	void update ();
	void render (Camera3D & camera);

	virtual ~Scene ()
	{
		if (this->geometry)
			delete this->geometry;
		if (this->point_lights)
			delete [] this->point_lights;
		if (this->materials)
			delete [] this->materials;
	}
};

# endif // __SOFTENGINE3D_CORE_SCENE_H
