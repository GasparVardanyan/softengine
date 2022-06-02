# include <utility>
# include <vector>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

void Camera3D :: put_pixel (point p, color4 c)
{
	if (p.x > 0 && p.y > 0 && p.x < renderer_cw && p.y < renderer_ch)
		renderer->draw ({p.x, p.y}, c);
}

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c)
{
	// TODO: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	// TODO: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm

	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	put_pixel ({(int) x, (int) y}, c);

	draw_line (x1, y1, x, y, c);
	draw_line (x, y, x2, y2, c);
}

vector3 Camera3D :: project (vector3 v)
{
	v = vector3_transform (v, projector);

	v.x = (v.x + .5) * renderer_cw;
	v.y = (-v.y + .5) * renderer_ch;

	return v;
}

void Camera3D :: draw_scan_line (int y, const vector3 * va, const vector3 * vb, const vector3 * vc, const vector3 * vd, color4 color)
{
}

void Camera3D :: draw_mesh (const Geometry & geometry, const matrix4 & transform)
{
	vector3 * vertices_projected = new vector3 [geometry.num_vertices];

	for (int i = 0; i < geometry.num_vertices; i++)
		vertices_projected [i] = project (vector3_transform (geometry.vertices [i], transform));

	unsigned long long colors [] = {
		0x0000FF,
		0x00FF00,
		0xFF0000,
		0xFFFF00,
		0xFF00FF,
		0x00FFFF
	};

	for (int i = 0; i < geometry.num_faces; i++)
	{
		// unsigned long long color = colors [i / 2];
		// unsigned char r = (color >> 16) & 0xFF;
		// unsigned char g = (color >> 8) & 0xFF;
		// unsigned char b = color & 0xFF;

		unsigned char r, g, b;
		r = g = b = (0.25f + (i % geometry.num_faces) * 0.75f / geometry.num_faces) * 0xFF;

		const vector3 * v1 = vertices_projected + geometry.faces [i].v1;
		const vector3 * v2 = vertices_projected + geometry.faces [i].v2;
		const vector3 * v3 = vertices_projected + geometry.faces [i].v3;

		if (v2->y < v1->y)
			std::swap (v1, v2);
		if (v3->y < v1->y)
			std::swap (v1, v3);
		if (v3->y < v2->y)
			std::swap (v2, v3);

		// v1 - top, v2 - mid, v3 - bot

		scalar_t yd32 = v3->y - v2->y;
		scalar_t yd31 = v3->y - v1->y;
		scalar_t yd21 = v2->y - v1->y;

		if (yd31)
		{
			if (yd21)
				for (int y = v1->y; y < v2->y; y++)
				{
					scalar_t sg = (y - v1->y) / yd31;
					scalar_t eg = (y - v1->y) / yd21;

					int sx = interpolate (v1->x, v3->x, sg);
					int ex = interpolate (v1->x, v2->x, eg);
					scalar_t sz = interpolate (v1->z, v3->z, sg);
					scalar_t ez = interpolate (v1->z, v2->z, eg);

					if (sx > ex)
					{
						std::swap (sx, ex);
						std::swap (sz, ez);
					}

					for (int x = sx; x <= ex; x++)
						if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
						{
							scalar_t z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
							renderer->draw ({x, y, z}, {r, g, b});
						}
				}

			if (yd32)
				for (int y = v2->y; y <= v3->y; y++)
				{
					scalar_t sg = (y - v1->y) / yd31;
					scalar_t eg = (v3->y - y) / yd32;
					int sx = interpolate (v1->x, v3->x, sg);
					int ex = interpolate (v3->x, v2->x, eg);
					scalar_t sz = interpolate (v1->z, v3->z, sg);
					scalar_t ez = interpolate (v3->z, v2->z, eg);

					if (sx > ex)
					{
						std::swap (sx, ex);
						std::swap (sz, ez);
					}

					for (int x = sx; x <= ex; x++)
						if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
						{
							scalar_t z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
							renderer->draw ({x, y, z}, {r, g, b});
						}
				}
		}
		// else
		// {
		//     int sx = v1->x;
		//     if (sx > v2->x)
		//         sx = v2->x;
		//     if (sx > v3->x)
		//         sx = v3->x;
		//
		//     int ex = v1->x;
		//     if (ex < v2->x)
		//         ex = v2->x;
		//     if (ex < v3->x)
		//         ex = v3->x;
		//
		//     int y = v1->y;
		//
		//     for (int x = sx; x <= ex; x++)
		//         if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
		//             renderer->draw ({x, y}, {0, 0, 0});
		// }
	}

	for (int i = 0; i < geometry.num_faces; i++)
	{
		break;
		const auto & v1 = vertices_projected [geometry.faces [i].v1];
		const auto & v2 = vertices_projected [geometry.faces [i].v2];
		const auto & v3 = vertices_projected [geometry.faces [i].v3];

		draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0, 0});
		draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0, 0});
		draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0, 0});
	}

	for (int i = 0; i < geometry.num_faces; i++)
	{
		break;
		const vector3 & v = vertices_projected [i];
		if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
			renderer->draw ({(int) v.x, (int) v.y}, {0xff, 0xff, 0xff});
	}

	delete [] vertices_projected;
}

void Camera3D :: render (Object3D * container, matrix4 transform)
{
	if (matrix4_equals (transform, {0}))
	{
		renderer->clear ();

		transform = MATRIX4_TRANSFORM (
			vector3_negative (position),
			vector3_negative (rotation),
			(vector3) {1, 1, 1}
		);
	}

	transform = matrix4_mul (
		MATRIX4_TRANSFORM (
			container->position,
			container->rotation,
			container->scale
		),
		transform
	);



	if (dynamic_cast <Mesh *> (container) != nullptr)
		draw_mesh (((Mesh *) container)->geometry, transform);

	for (auto obj : container->children)
		render (obj.get (), transform);
}
