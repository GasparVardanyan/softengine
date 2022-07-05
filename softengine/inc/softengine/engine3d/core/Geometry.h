# ifndef __SOFTENGINE3D_CORE_GEOMETRY_H
# define __SOFTENGINE3D_CORE_GEOMETRY_H

# include <cstddef>

# include "softengine/math.h"

struct face
{
	std::size_t v1, v2, v3;
	int textureindex;
};

struct uv
{
	scalar_t u, v;
};

struct vertex
{
	vector3 position;
	vector3 normal;
	uv texture_coordinates;
};

// TODO: implement surfaces

class Geometry
{
public:
	vertex * vertices;
	face * faces;

	std::size_t num_vertices;
	std::size_t num_faces;

	Geometry (std::size_t num_vertices, std::size_t num_faces)
	{
		this->num_vertices = num_vertices;
		this->num_faces = num_faces;

		this->vertices = new vertex [num_vertices];
		this->faces = new face [num_faces];
	}

	void transform (const matrix4 & transform)
	{
		for (int i = 0; i < this->num_vertices; i++)
			this->vertices [i] = {
				vector3_transform (this->vertices [i].position, transform),
				vector3_transform_normal (this->vertices [i].normal, transform)
			};
	}

	virtual ~Geometry ()
	{
		delete [] this->vertices;
		delete [] this->faces;
	}
};

# endif // __SOFTENGINE3D_CORE_GEOMETRY_H
