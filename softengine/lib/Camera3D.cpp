# include <utility>
# include <vector>
# include <iostream>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c)
{
	// TODO: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	// TODO: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm

	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	int view_width = renderer -> canvas_width ();
	int view_height = renderer -> canvas_height ();

	if (x > 0 && y > 0 && x < view_width && y < view_height)
		renderer -> draw ({(int) x, (int) y}, c);

	draw_line (x1, y1, x, y, c);
	draw_line (x, y, x2, y2, c);
}

void Camera3D :: render (Object3D * container, matrix4 transform)
{
	if (matrix4_equals (transform, MATRIX4_ZERO))
	{
		renderer -> clear ();

		transform = MATRIX4_TRANSFORM (
			vector3_negative (position),
			vector3_negative (rotation),
			(vector3) {1, 1, 1}
		);
	}

	transform = matrix4_mul (
		MATRIX4_TRANSFORM (
			container -> position,
			container -> rotation,
			container -> scale
		),
		transform
	);

	matrix4 draw_matrix = matrix4_mul (transform, projector);



	if (dynamic_cast <Mesh *> (container) != nullptr)
	{
		Mesh * m = (Mesh *) container;
		std::vector <point> vertices_projected;

		int view_width = renderer -> canvas_width ();
		int view_height = renderer -> canvas_height ();



		for (auto v : m -> geometry.vertices)
		{
			vector3 vert = vector3_transform (v, draw_matrix);

			vert.x = (vert.x + .5) * view_width;
			vert.y = (vert.y + .5) * view_height;

			vertices_projected.push_back ({(int) vert.x, (int) vert.y});

			if (vert.x > 0 && vert.y > 0 && vert.x < view_width && vert.y < view_height)
				renderer -> draw ({(int) vert.x, (int) vert.y}, {0xff, 0, 0});
		}

		for (auto f : m -> geometry.faces)
		{
			auto v1 = vertices_projected [f.v1];
			auto v2 = vertices_projected [f.v2];
			auto v3 = vertices_projected [f.v3];



			// triangles:

			if (v2.y < v1.y)
				std::swap (v1, v2);
			if (v3.y < v1.y)
				std::swap (v1, v3);
			if (v3.y < v2.y)
				std::swap (v2, v3);

			if (v1.y > v2.y || v2.y > v3.y) std::cout << "FFFFFFFF" << std::endl;

			auto * vl = & v2;
			auto * vr = & v3;

			if (vl -> x > vr -> x)
				std::swap (vl, vr);

			scalar_t sl = (v1.y - vl -> y) / (v1.x - vl -> x);
			scalar_t sr = (v1.y - vr -> y) / (v1.x - vr -> x);

# if 1
			draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0xff, 0});
			draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0xff, 0});
			draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0xff, 0});
# endif
		}
	}

	for (auto obj : container -> children)
		render (obj.get (), transform);
}
