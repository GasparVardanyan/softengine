# ifndef __SOFTENGINE_CORE_GEOMETRY_H
# define __SOFTENGINE_CORE_GEOMETRY_H

# include "softengine/math.h"

class Geometry
{
protected:
		bool created;
public:
	vertex * vertices;
	face * faces;

	int num_vertices;
	int num_faces;

	Geometry ()
		: vertices (nullptr)
		, faces (nullptr)
		, num_vertices (0)
		, num_faces (0)
		, created (false)
	{}

	void create (int num_vertices, int num_faces)
	{
		destroy ();

		this -> num_vertices = num_vertices;
		this -> num_faces = num_faces;

		this -> vertices = new vertex [num_vertices];
		this -> faces = new face [num_faces];

		this -> created = true;
	}

	void destroy ()
	{
		if (this -> created)
		{
			delete [] vertices;
			delete [] faces;

			num_vertices = 0;
			num_faces = 0;
			created = false;
		}
	}

	virtual ~Geometry ()
	{
		destroy ();
	}
};

# endif // __SOFTENGINE_CORE_GEOMETRY_H
