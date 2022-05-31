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
		point * vertices_projected = new point [m -> geometry.num_vertices];

		int view_width = renderer -> canvas_width ();
		int view_height = renderer -> canvas_height ();

		for (int i = 0; i < m -> geometry.num_vertices; i++)
		{
			vector3 vert = vector3_transform (m -> geometry.vertices [i], draw_matrix);

			vert.x = (vert.x + .5) * view_width;
			vert.y = (-vert.y + .5) * view_height;

			vertices_projected [i] = {(int) vert.x, (int) vert.y};
		}

		for (int i = 0; i < m -> geometry.num_faces; i++)
		{
			const point * v1 = vertices_projected + m -> geometry.faces [i].v1;
			const point * v2 = vertices_projected + m -> geometry.faces [i].v2;
			const point * v3 = vertices_projected + m -> geometry.faces [i].v3;

			if (v2 -> y < v1 -> y)
				std::swap (v1, v2);
			if (v3 -> y < v1 -> y)
				std::swap (v1, v3);
			if (v3 -> y < v2 -> y)
				std::swap (v2, v3);

			// v1 - top, v2 - mid, v3 - bot

			int yd32 = v3 -> y - v2 -> y;
			int yd31 = v3 -> y - v1 -> y;
			int yd21 = v2 -> y - v1 -> y;

			if (yd31)
			{
				if (yd21)
					for (int y = v1 -> y; y < v2 -> y; y++)
					{
						int sx = v1 -> x + (v3 -> x - v1 -> x) * (y - v1 -> y) / yd31;
						int ex = v1 -> x + (v2 -> x - v1 -> x) * (y - v1 -> y) / yd21;

						if (sx > ex) std::swap (sx, ex);

						for (int x = sx; x <= ex; x++)
							if (x > 0 && y > 0 && x < view_width && y < view_height)
								renderer -> draw ({x, y}, {0, 0, 0});
					}

				if (yd32)
					for (int y = v2 -> y; y <= v3 -> y; y++)
					{
						int sx = v1 -> x + (v3 -> x - v1 -> x) * (y - v1 -> y) / yd31;
						int ex = v3 -> x + (v2 -> x - v3 -> x) * (v3 -> y - y) / yd32;

						if (sx > ex) std::swap (sx, ex);

						for (int x = sx; x <= ex; x++)
							if (x > 0 && y > 0 && x < view_width && y < view_height)
								renderer -> draw ({x, y}, {0, 0, 0});
					}
			}
			// else
			// {
			//     int sx = v1 -> x;
			//     if (sx > v2 -> x)
			//         sx = v2 -> x;
			//     if (sx > v3 -> x)
			//         sx = v3 -> x;
			//
			//     int ex = v1 -> x;
			//     if (ex < v2 -> x)
			//         ex = v2 -> x;
			//     if (ex < v3 -> x)
			//         ex = v3 -> x;
			//
			//     int y = v1 -> y;
			//
			//     for (int x = sx; x <= ex; x++)
			//         if (x > 0 && y > 0 && x < view_width && y < view_height)
			//             renderer -> draw ({x, y}, {0, 0, 0});
			// }
		}

		for (int i = 0; i < m -> geometry.num_faces; i++)
		{
			const auto & v1 = vertices_projected [m -> geometry.faces [i].v1];
			const auto & v2 = vertices_projected [m -> geometry.faces [i].v2];
			const auto & v3 = vertices_projected [m -> geometry.faces [i].v3];

			draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0xff, 0});
			draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0xff, 0});
			draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0xff, 0});
		}

		for (int i = 0; i < m -> geometry.num_faces; i++)
		{
			const point & v = vertices_projected [i];
			if (v.x > 0 && v.y > 0 && v.x < view_width && v.y < view_height)
				renderer -> draw ({v.x, v.y}, {0xff, 0, 0});
		}

		delete [] vertices_projected;
	}

	for (auto obj : container -> children)
		render (obj.get (), transform);
}
