# include <math.h>

# include "softengine/engine3d/primitives/Box.h"

Box::Box (scalar_t width, scalar_t height, scalar_t length, matrix4 transform)
{
	scalar_t _w = width / 2;
	scalar_t _h = height / 2;
	scalar_t _l = length / 2;

	this->geometry = new Geometry (8, 12);
	scalar_t n = std::sqrt (1 / 3.l);
	// scalar_t n = 0.5773502691896257ll;

	this->geometry->vertices [0] =  {{-_w,  _l,  _h}, {-n,  n,  n}};
	this->geometry->vertices [1] =  {{ _w,  _l,  _h}, { n,  n,  n}};
	this->geometry->vertices [2] =  {{-_w, -_l,  _h}, {-n, -n,  n}};
	this->geometry->vertices [3] =  {{ _w, -_l,  _h}, { n, -n,  n}};
	this->geometry->vertices [4] =  {{-_w,  _l, -_h}, {-n,  n, -n}};
	this->geometry->vertices [5] =  {{ _w,  _l, -_h}, { n,  n, -n}};
	this->geometry->vertices [6] =  {{ _w, -_l, -_h}, { n, -n, -n}};
	this->geometry->vertices [7] =  {{-_w, -_l, -_h}, {-n, -n, -n}};

	if (!matrix4_equals (transform, {0}))
		this->geometry->transform (transform);

	this->geometry->faces [ 0] = { 0, 1, 2 };
	this->geometry->faces [ 1] = { 1, 2, 3 };
	this->geometry->faces [ 2] = { 1, 3, 6 };
	this->geometry->faces [ 3] = { 1, 5, 6 };
	this->geometry->faces [ 4] = { 0, 1, 4 };
	this->geometry->faces [ 5] = { 1, 4, 5 };
	this->geometry->faces [ 6] = { 2, 3, 7 };
	this->geometry->faces [ 7] = { 3, 6, 7 };
	this->geometry->faces [ 8] = { 0, 2, 7 };
	this->geometry->faces [ 9] = { 0, 4, 7 };
	this->geometry->faces [10] = { 4, 5, 6 };
	this->geometry->faces [11] = { 4, 6, 7 };
}
