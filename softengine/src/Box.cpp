# include <math.h>

# include "softengine/engine3d/primitives/Box.h"

Box::Box (scalar_t width, scalar_t height, scalar_t length, matrix4 transform)
{
	scalar_t _w = width / 2;
	scalar_t _h = height / 2;
	scalar_t _l = length / 2;

	this->geometry.create (8, 12);
	scalar_t n = std::sqrt (1 / 3.l);

	this->geometry.vertices.push_back ({{-_w,  _l,  _h}, {-n,  n,  n}});
	this->geometry.vertices.push_back ({{ _w,  _l,  _h}, { n,  n,  n}});
	this->geometry.vertices.push_back ({{-_w, -_l,  _h}, {-n, -n,  n}});
	this->geometry.vertices.push_back ({{ _w, -_l,  _h}, { n, -n,  n}});
	this->geometry.vertices.push_back ({{-_w,  _l, -_h}, {-n,  n, -n}});
	this->geometry.vertices.push_back ({{ _w,  _l, -_h}, { n,  n, -n}});
	this->geometry.vertices.push_back ({{ _w, -_l, -_h}, { n, -n, -n}});
	this->geometry.vertices.push_back ({{-_w, -_l, -_h}, {-n, -n, -n}});

	if (!matrix4_equals (transform, {0}))
		this->geometry.transform (transform);

	this->geometry.faces.push_back ({ 0, 1, 2 });
	this->geometry.faces.push_back ({ 1, 2, 3 });
	this->geometry.faces.push_back ({ 1, 3, 6 });
	this->geometry.faces.push_back ({ 1, 5, 6 });
	this->geometry.faces.push_back ({ 0, 1, 4 });
	this->geometry.faces.push_back ({ 1, 4, 5 });
	this->geometry.faces.push_back ({ 2, 3, 7 });
	this->geometry.faces.push_back ({ 3, 6, 7 });
	this->geometry.faces.push_back ({ 0, 2, 7 });
	this->geometry.faces.push_back ({ 0, 4, 7 });
	this->geometry.faces.push_back ({ 4, 5, 6 });
	this->geometry.faces.push_back ({ 4, 6, 7 });
}
