# include <math.h>

# include "softengine/engine3d/primitives/Box.h"

Box::Box (scalar_t width, scalar_t height, scalar_t length, matrix4 transform)
{
	scalar_t _w = width / 2;
	scalar_t _h = height / 2;
	scalar_t _l = length / 2;

	this->geometry.create (8, 12);
	scalar_t n = std::sqrt (1 / 3.l);

	this->geometry.vertices [0].position = {-_w,  _l,  _h};
	this->geometry.vertices [1].position = { _w,  _l,  _h};
	this->geometry.vertices [2].position = {-_w, -_l,  _h};
	this->geometry.vertices [3].position = { _w, -_l,  _h};
	this->geometry.vertices [4].position = {-_w,  _l, -_h};
	this->geometry.vertices [5].position = { _w,  _l, -_h};
	this->geometry.vertices [6].position = { _w, -_l, -_h};
	this->geometry.vertices [7].position = {-_w, -_l, -_h};
	this->geometry.vertices [0].normal = {-n,  n,  n};
	this->geometry.vertices [1].normal = { n,  n,  n};
	this->geometry.vertices [2].normal = {-n, -n,  n};
	this->geometry.vertices [3].normal = { n, -n,  n};
	this->geometry.vertices [4].normal = {-n,  n, -n};
	this->geometry.vertices [5].normal = { n,  n, -n};
	this->geometry.vertices [6].normal = { n, -n, -n};
	this->geometry.vertices [7].normal = {-n, -n, -n};

	if (!matrix4_equals (transform, {0}))
		for (int i = 0; i < this->geometry.num_vertices; i++)
			this->geometry.vertices [i].position = vector3_transform (
				this->geometry.vertices [i].position,
				transform
			);

	this->geometry.faces [ 0] = { 0, 1, 2 };
	this->geometry.faces [ 1] = { 1, 2, 3 };
	this->geometry.faces [ 2] = { 1, 3, 6 };
	this->geometry.faces [ 3] = { 1, 5, 6 };
	this->geometry.faces [ 4] = { 0, 1, 4 };
	this->geometry.faces [ 5] = { 1, 4, 5 };
	this->geometry.faces [ 6] = { 2, 3, 7 };
	this->geometry.faces [ 7] = { 3, 6, 7 };
	this->geometry.faces [ 8] = { 0, 2, 7 };
	this->geometry.faces [ 9] = { 0, 4, 7 };
	this->geometry.faces [10] = { 4, 5, 6 };
	this->geometry.faces [11] = { 4, 6, 7 };
}
