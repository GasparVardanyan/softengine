# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

# include <vector>
# include <utility>

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, unsigned char r, unsigned char g, unsigned char b)
{
	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	int view_width = renderer -> canvas_width ();
	int view_height = renderer -> canvas_height ();

	if (x > 0 && y > 0 && x < view_width && y < view_height)
		renderer -> draw ((int) x, (int) y, r, g, b);

	draw_line (x1, y1, x, y, r, g, b);
	draw_line (x, y, x2, y2, r, g, b);
}

void Camera3D :: render (Object3D * container, matrix4 transform)
{
	if (matrix4_equals (transform, MATRIX4_ZERO))
	{
		renderer -> clear ();
		transform = MATRIX4_TRANSFORM (position, rotation, scale);
	}

	// p2 * cam
	// p1 * p2 * cam
	// obj * p1 * p2 * cam
	//   obj * p1 * p2 * cam * pr


	transform = matrix4_mul (MATRIX4_TRANSFORM (container -> position, container -> rotation, container -> scale), transform);

	matrix4 draw_matrix = matrix4_mul (transform, projector);



	if (dynamic_cast <Mesh *> (container) != nullptr)
	{
		Mesh * m = (Mesh *) container;
		std::vector <std::pair <scalar_t, scalar_t>> vertices_projected;

		int view_width = renderer -> canvas_width ();
		int view_height = renderer -> canvas_height ();



		for (auto v : m -> geometry.vertices)
		{
			vector3 vert = vector3_transform (v, draw_matrix);

			vert.x = (vert.x + .5) * view_width;
			vert.y = (vert.y + .5) * view_height;

			vertices_projected.push_back ({vert.x, vert.y});

			// if (vert.x > 0 && vert.y > 0 && vert.x < view_width && vert.y < view_height)
			//     renderer -> draw ((int) vert.x, (int) vert.y, 0xff, 0, 0);
		}

		for (auto f : m -> geometry.faces)
		{
			auto v1 = vertices_projected [f.x];
			auto v2 = vertices_projected [f.y];
			auto v3 = vertices_projected [f.z];

			draw_line (v1.first, v1.second, v2.first, v2.second, 0, 0xff, 0);
			draw_line (v2.first, v2.second, v3.first, v3.second, 0, 0xff, 0);
			draw_line (v3.first, v3.second, v1.first, v1.second, 0, 0xff, 0);
		}
	}

	for (auto obj : container -> children)
		render (obj.get (), transform);
}
