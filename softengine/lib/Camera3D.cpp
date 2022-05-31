# include <iostream>
# include <limits>
# include <utility>
# include <vector>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c)
{
	// TODO: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	// TODO: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm

	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
		renderer -> draw ({(int) x, (int) y}, c);

	draw_line (x1, y1, x, y, c);
	draw_line (x, y, x2, y2, c);
}

void Camera3D :: clear_depth_buffer ()
{
	for (int i = 0; i < renderer_cw * renderer_ch; i++)
		depth_buffer [i] = std::numeric_limits <scalar_t> :: infinity ();
}

void Camera3D :: render (Object3D * container, matrix4 transform)
{
	if (matrix4_equals (transform, MATRIX4_ZERO))
	{
		renderer -> clear ();
		clear_depth_buffer ();

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



	if (dynamic_cast <Mesh *> (container) != nullptr)
	{
		Geometry * g = & ((Mesh *) container) -> geometry;
		point * vertices_projected = new point [g -> num_vertices];

		for (int i = 0; i < g -> num_vertices; i++)
		{
			vector3 vert = vector3_transform (g -> vertices [i], transform);
			scalar_t _z = vert.z;
			vert = vector3_transform (vert, projector);

			vert.x = (vert.x + .5) * renderer_cw;
			vert.y = (-vert.y + .5) * renderer_ch;

			vertices_projected [i] = {(int) vert.x, (int) vert.y, _z};
		}

		for (int i = 0; i < g -> num_faces; i++)
		{
			unsigned char color = (0.25f + (i % g -> num_faces) * 0.75f / g -> num_faces) * 0xFF;

			const point * v1 = vertices_projected + g -> faces [i].v1;
			const point * v2 = vertices_projected + g -> faces [i].v2;
			const point * v3 = vertices_projected + g -> faces [i].v3;

			if (v2 -> y < v1 -> y)
				std::swap (v1, v2);
			if (v3 -> y < v1 -> y)
				std::swap (v1, v3);
			if (v3 -> y < v2 -> y)
				std::swap (v2, v3);

			// v1 - top, v2 - mid, v3 - bot

			scalar_t yd32 = v3 -> y - v2 -> y; // yes, scalar_t instead of int
			scalar_t yd31 = v3 -> y - v1 -> y;
			scalar_t yd21 = v2 -> y - v1 -> y;

			if (yd31)
			{
				if (yd21)
					for (int y = v1 -> y; y < v2 -> y; y++)
					{
						scalar_t sg = (y - v1 -> y) / yd31;
						scalar_t eg = (y - v1 -> y) / yd21;

						int sx = interpolate (v1 -> x, v3 -> x, sg);
						int ex = interpolate (v1 -> x, v2 -> x, eg);
						scalar_t sz = interpolate (v1 -> _z, v3 -> _z, sg);
						scalar_t ez = interpolate (v1 -> _z, v2 -> _z, eg);

						if (sx > ex) std::swap (sx, ex);
						if (sz > ez) std::swap (sz, ez);

						for (int x = sx; x <= ex; x++)
							if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
							{
								scalar_t z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
								if (z < depth_buffer [y * renderer_cw + x])
								{
									depth_buffer [y * renderer_cw + x] = z;
									renderer -> draw ({x, y}, {color, color, color});
								}
							}
					}

				if (yd32)
					for (int y = v2 -> y; y <= v3 -> y; y++)
					{
						scalar_t sg = (y - v1 -> y) / yd31;
						scalar_t eg = (v3 -> y - y) / yd32;
						int sx = interpolate (v1 -> x, v3 -> x, sg);
						int ex = interpolate (v3 -> x, v2 -> x, eg);
						scalar_t sz = interpolate (v1 -> _z, v3 -> _z, sg);
						scalar_t ez = interpolate (v3 -> _z, v2 -> _z, eg);

						if (sx > ex) std::swap (sx, ex);
						if (sz > ez) std::swap (sz, ez);

						for (int x = sx; x <= ex; x++)
							if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
							{
								scalar_t z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
								if (z < depth_buffer [y * renderer_cw + x])
								{
									depth_buffer [y * renderer_cw + x] = z;
									renderer -> draw ({x, y}, {color, color, color});
								}
							}
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
			//         if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
			//             renderer -> draw ({x, y}, {0, 0, 0});
			// }
		}

		// for (int i = 0; i < g -> num_faces; i++)
		// {
		//     const auto & v1 = vertices_projected [g -> faces [i].v1];
		//     const auto & v2 = vertices_projected [g -> faces [i].v2];
		//     const auto & v3 = vertices_projected [g -> faces [i].v3];
        //
		//     draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0xff, 0});
		//     draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0xff, 0});
		//     draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0xff, 0});
		// }
        //
		// for (int i = 0; i < g -> num_faces; i++)
		// {
		//     const point & v = vertices_projected [i];
		//     if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
		//         renderer -> draw ({v.x, v.y}, {0xff, 0, 0});
		// }

		delete [] vertices_projected;
	}

	for (auto obj : container -> children)
		render (obj.get (), transform);
}
