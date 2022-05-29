# include "softengine/engine3d/primitives/Box.h"

Box::Box (scalar_t width, scalar_t height, scalar_t length, matrix4 transform)
{
	scalar_t _w = width / 2;
	scalar_t _h = height / 2;
	scalar_t _l = length / 2;

	this -> geometry.vertices = std::vector <vector3>
	{
		{-_w,  _l,  _h},
		{ _w,  _l,  _h},
		{-_w, -_l,  _h},
		{ _w, -_l,  _h},
		{-_w,  _l, -_h},
		{ _w,  _l, -_h},
		{ _w, -_l, -_h},
		{-_w, -_l, -_h}
	};

	if (!matrix4_equals (transform, {0}))
		for (auto & v : this -> geometry.vertices)
			v = vector3_transform (v, transform);

	this -> geometry.faces = std::vector <vector3>
	{
		{ 0, 1, 2 },
		{ 1, 2, 3 },
		{ 1, 3, 6 },
		{ 1, 5, 6 },
		{ 0, 1, 4 },
		{ 1, 4, 5 },
		{ 2, 3, 7 },
		{ 3, 6, 7 },
		{ 0, 2, 7 },
		{ 0, 4, 7 },
		{ 4, 5, 6 },
		{ 4, 6, 7 }
	};
}
