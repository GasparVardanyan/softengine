# include <math.h>

# include "softengine/engine3d/primitives/Plane.h"

Plane::Plane (scalar_t width, scalar_t height, bool twosided, matrix4 transform)
{

	// TODO: set uv coordinates

	scalar_t _w = width / 2;
	scalar_t _h = height / 2;

	this->geometry = new Geometry (twosided ? 8 : 4, twosided ? 4 : 2);
	static const scalar_t n = std::sqrt (1 / 3.l);
	// scalar_t n = 0.5773502691896257ll;

	this->geometry->vertices [0] =  {{-_w,  0,  _h}, {-n,  n,  n}, {0, 1}};
	this->geometry->vertices [1] =  {{ _w,  0,  _h}, { n,  n,  n}, {1, 1}};
	this->geometry->vertices [2] =  {{-_w,  0, -_h}, {-n,  n, -n}, {0, 0}};
	this->geometry->vertices [3] =  {{ _w,  0, -_h}, { n,  n, -n}, {1, 0}};
	if (twosided)
	{
		this->geometry->vertices [4] =  {{-_w,  0,  _h}, {-n, -n,  n}, {0, 1}};
		this->geometry->vertices [5] =  {{ _w,  0,  _h}, { n, -n,  n}, {1, 1}};
		this->geometry->vertices [6] =  {{-_w,  0, -_h}, {-n, -n, -n}, {0, 0}};
		this->geometry->vertices [7] =  {{ _w,  0, -_h}, { n, -n, -n}, {1, 0}};
	}

	if (!matrix4_equals (transform, {0}))
		this->geometry->transform (transform);

	this->geometry->faces [0] = { 0, 1, 2 };
	this->geometry->faces [1] = { 1, 2, 3 };
	if (twosided)
	{
		this->geometry->faces [2] = { 6, 5, 4 };
		this->geometry->faces [3] = { 7, 6, 5 };
	}
}
