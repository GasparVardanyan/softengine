# ifndef __SOFTENGINE_CORE_GEOMETRY_H
# define __SOFTENGINE_CORE_GEOMETRY_H

# include <vector>

# include "softengine/math.h"

struct face
{
	std::size_t v1, v2, v3;
};

struct vertex
{
	vector3 position;
	vector3 normal;
};

class Geometry
{
public:
	std::vector <vertex> vertices;
	std::vector <face> faces;

	std::size_t num_vertices;
	std::size_t num_faces;

	Geometry ()
		: vertices (0)
		, faces (0)
		, num_vertices (0)
		, num_faces (0)
	{}

	void create (std::size_t num_vertices = 0, std::size_t num_faces = 0)
	{
		this->num_vertices = num_vertices;
		this->num_faces = num_faces;

		this->vertices.reserve (num_vertices);
		this->faces.reserve (num_faces);
	}

	void update_sizes ()
	{
		this->num_vertices = this->vertices.size ();
		this->num_faces = this->faces.size ();
	}

	void transform (const matrix4 & transform)
	{
		// TODO: transform normals
		for (int i = 0; i < this->num_vertices; i++)
			this->vertices [i].position = vector3_transform (
				this->vertices [i].position,
				transform
			);
	}

	void destroy ()
	{
		this->num_vertices = 0;
		this->num_faces = 0;

		this->vertices.clear ();
		this->faces.clear ();
	}
};

# endif // __SOFTENGINE_CORE_GEOMETRY_H
