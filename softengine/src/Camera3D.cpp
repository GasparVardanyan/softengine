# include <algorithm>
# include <utility>
# include <vector>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

struct vertex_data
{
	vector3 position;
	vector3 normal;
	union {
		vector3 projected;
		struct {
			scalar_t x, y, z;
		};
	};
};

void Camera3D :: put_pixel (point p, color4 c)
{
	if (p.x > 0 && p.y > 0 && p.x < renderer_cw && p.y < renderer_ch)
		renderer->put_pixel ({p.x, p.y}, c);
}

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c)
{
	// TODO: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	// TODO: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm

	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	put_pixel ({(int) x, (int) y, 0}, c);

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

void Camera3D :: render (const Scene & scene)
{
	renderer->clear_canvas ();
	// std::fill (depth_buffer.begin (), depth_buffer.end (), std::numeric_limits <scalar_t> :: infinity ());
	// std::fill (bmp, { 0xff, 0x80, 0x00 });
	for (int i = 0; i < renderer_cs; i++);

	matrix4 transform = MATRIX4_TRANSFORM (
		vector3_negative (position),
		vector3_negative (rotation),
		(vector3) {1, 1, 1}
	);

	const Geometry & geometry = scene.geometry;

	vertex_data * vertices_projected = new vertex_data [geometry.num_vertices];

	/* TODO: OPTIMISATIONS TO BE DONE:
	 *
	 * 1) avoid processing unnecessary faces (first check the depth buffer)
	 * 2) avoid calculating unnecessary normals (first check the depth buffer)
	 *
	 * ... not per mesh, but rootContainer-wise, so... first combina all
	 * drawable geometry in a signe mesh, then call draw_mesh for the result !!
	 */

	for (int i = 0; i < geometry.num_vertices; i++)
	{
		vertices_projected [i] = (vertex_data) {
			.position = vector3_transform (geometry.vertices [i].position, transform),
			.normal = vector3_transform_normal (geometry.vertices [i].normal, transform)
		};
		vertices_projected [i].projected = project (vertices_projected [i].position);
	}

	for (int i = 0; i < geometry.num_faces; i++)
	{
		// unsigned char r, g, b;
		// r = g = b = (0.25f + (i % geometry.num_faces) * 0.75f / geometry.num_faces) * 0xFF;

		// Current face vertices projected
		const vertex_data * v1 = vertices_projected + geometry.faces [i].v1;
		const vertex_data * v2 = vertices_projected + geometry.faces [i].v2;
		const vertex_data * v3 = vertices_projected + geometry.faces [i].v3;

		// Point v1 on top, v2 on middle and v3 on bottom projected vertices
		if (v2->y < v1->y)
			std::swap (v1, v2);
		if (v3->y < v1->y)
			std::swap (v1, v3);
		if (v3->y < v2->y)
			std::swap (v2, v3);

		scalar_t yd23 = v2->y - v3->y;
		scalar_t yd31 = v3->y - v1->y;
		scalar_t yd21 = v2->y - v1->y;

// DRAW_SCAN_LINE

		int y;
		const vertex_data * va, * vb, * vc, * vd;
		scalar_t yds, yde;

		int y1 = v1->y;
		int y2 = v2->y;
		int y3 = v3->y;

		if (y3 < 0 || y1 >= renderer_ch)
			continue;

		if (y1 < 0)
		{
			y1 = 0;
			if (y2 < 0)
				y2 = 0;
		}
		if (y3 >= renderer_ch)
		{
			y3 = renderer_ch;
			if (y2 >= renderer_ch)
				y2 = renderer_ch;
		}

		// v1, v2, v3 - top, mid, bot



		vector3 light = {0, 10, -3};

		scalar_t nal1 = vector3_dot (
			v1 -> normal,
			vector3_normalized (
				vector3_sub (
					light,
					v1 -> position
				)
			)
		);
		scalar_t nal2 = vector3_dot (
			v2 -> normal,
			vector3_normalized (
				vector3_sub (
					light,
					v2 -> position
				)
			)
		);
		scalar_t nal3 = vector3_dot (
			v3 -> normal,
			vector3_normalized (
				vector3_sub (
					light,
					v3 -> position
				)
			)
		);

		scalar_t nala, nalb, nalc, nald;

		va = v1, vb = v3, vc = v1, vd = v2, yds = yd31, yde = yd21;
		nala = nal1, nalb = nal3, nalc = nal1, nald = nal2;

		int ys = y1, ye = y2;

draw_scan_line:
		for (y = ys; y <= ye; y++)
		{
			scalar_t sg = yds ? (y - va->y) / yds : 1;
			scalar_t eg = yde ? (y - vc->y) / yde : 1;

			int sx = interpolate (va->x, vb->x, sg);
			int ex = interpolate (vc->x, vd->x, eg);
			scalar_t sz = interpolate (va->z, vb->z, sg);
			scalar_t ez = interpolate (vc->z, vd->z, eg);

			scalar_t nals = interpolate (nala, nalb, sg);
			scalar_t nale = interpolate (nalc, nald, eg);

			if (sx > ex)
			{
				std::swap (sx, ex);
				std::swap (sz, ez);
				std::swap (nals, nale);
			}

			if (sx < 0) sx = 0;
			if (ex > renderer_cw - 1) ex = renderer_cw - 1;

			int x;
			scalar_t z;

			vector3 c, n;
			unsigned char color;

			// per face computations there...

			// TODO: do this in Scene.cpp

			// c = vector3_scale (
			//     vector3_add (
			//         geometry.vertices [geometry.faces [i].v1].position,
			//         vector3_add (
			//             geometry.vertices [geometry.faces [i].v2].position,
			//             geometry.vertices [geometry.faces [i].v3].position
			//         )
			//     ),
			//     1.l / 3
			// );
			//
			// n = vector3_scale (
			//     vector3_add (
			//         geometry.vertices [geometry.faces [i].v1].normal,
			//         vector3_add (
			//             geometry.vertices [geometry.faces [i].v2].normal,
			//             geometry.vertices [geometry.faces [i].v3].normal
			//         )
			//     ),
			//     1.l / 3
			// );
			//
			// vector3 l = vector3_sub (light, c);
			// scalar_t cosa = vector3_cos (n, l);
			// scalar_t a = clamp (cosa, 0, 1);
			// color = a * 0xFF;

			// vector3 v = project (vector3_transform (c, transform));
			// if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
			//     renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0, 0});

			for (x = sx; x <= ex; x++)
			{
				z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
				scalar_t nal = interpolate (nals, nale, (scalar_t) (x - sx) / (ex - sx));
				color = clamp (nal, 0, 1) * 0xFF;
				if (renderer->check_depth_buffer ({x, y, z}))
				{
					renderer->put_pixel ({x, y, z}, {color, color, color});
				}
			}
		}

		if (y < y3)
		{
			vc = v3, yde = yd23;
			ys = y2; ye = y3;
			nalc = nal3;
			goto draw_scan_line;
		}
	}

	for (int i = 0; i < geometry.num_faces; i++)
	{
		break;

		const vertex_data & v1 = vertices_projected [geometry.faces [i].v1];
		const vertex_data & v2 = vertices_projected [geometry.faces [i].v2];
		const vertex_data & v3 = vertices_projected [geometry.faces [i].v3];

		draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0, 0});
		draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0, 0});
		draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0, 0});

		// vector3 c = vector3_scale ((vector3) {
		//     vertices_projected [geometry.faces [i].v1].position.x + vertices_projected [geometry.faces [i].v2].position.x + vertices_projected [geometry.faces [i].v3].position.x,
		//     vertices_projected [geometry.faces [i].v1].position.y + vertices_projected [geometry.faces [i].v2].position.y + vertices_projected [geometry.faces [i].v3].position.y,
		//     vertices_projected [geometry.faces [i].v1].position.z + vertices_projected [geometry.faces [i].v2].position.z + vertices_projected [geometry.faces [i].v3].position.z
		// }, 1./3);
        //
		// vector3 v = project (c);
		// if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
		//     renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0, 0});
	}

	for (int i = 0; i < geometry.num_vertices; i++)
	{
		break;
		const vertex_data & v = vertices_projected [i];
		if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
			renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0xff, 0xff});
	}

	delete [] vertices_projected;
}
