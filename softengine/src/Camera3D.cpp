# include <algorithm>
# include <utility>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

struct vertex_data
{
	vector3 world_pos;
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
	matrix4 transform = MATRIX4_TRANSFORM (
		vector3_negative (position),
		vector3_negative (rotation),
		(vector3) {1, 1, 1}
	); // TODO: URGENT!! fix camera transform

	v = vector3_transform (v, matrix4_mul (transform, projector));

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

	const Geometry * geometry = scene.geometry;

	vertex_data * vertices_projected = new vertex_data [geometry->num_vertices];

	/* TODO: OPTIMISATIONS TO BE DONE:
	 *
	 * 1) avoid processing unnecessary faces (first check the depth buffer)
	 * 2) avoid calculating unnecessary normals (first check the depth buffer)
	 *
	 * ... not per mesh, but rootContainer-wise, so... first combina all
	 * drawable geometry in a signe mesh, then call draw_mesh for the result !!
	 */

	for (int i = 0; i < geometry->num_vertices; i++)
	{
		vertices_projected [i] = (vertex_data) {
			.world_pos = geometry->vertices [i].position,
			.normal = geometry->vertices [i].normal,
			.projected = project (geometry->vertices [i].position)
		};
	}

	for (int i = 0; i < geometry->num_faces; i++)
	{
		// unsigned char r, g, b;
		// r = g = b = (0.25f + (i % geometry->num_faces) * 0.75f / geometry->num_faces) * 0xFF;
		unsigned char color = (0.25f + (i % geometry->num_faces) * 0.75f / geometry->num_faces) * 0xFF;
		// color=0xFF;

		// Current face vertices projected
		const vertex_data * v1 = vertices_projected + geometry->faces [i].v1;
		const vertex_data * v2 = vertices_projected + geometry->faces [i].v2;
		const vertex_data * v3 = vertices_projected + geometry->faces [i].v3;

// # define DBG
# ifdef DBG

		const vertex_data __v [] = {
			{.x = 40, .y = 200},
			{.x = 210, .y = 270},
			{.x = 100, .y = 100},
		};

		v1 = __v, v2 = __v + 1, v3 = __v + 2;

# endif // DBG

		// Point v1 on top, v2 on middle and v3 on bottom projected vertices
		if (v2->y < v1->y)
			std::swap (v1, v2);
		if (v3->y < v1->y)
			std::swap (v1, v3);
		if (v3->y < v2->y)
			std::swap (v2, v3);

		scalar_t yd32 = v2->y - v3->y;
		scalar_t yd31 = v1->y - v3->y;
		scalar_t yd21 = v1->y - v2->y;
		scalar_t xd32 = v2->x - v3->x;
		scalar_t xd31 = v1->x - v3->x;
		scalar_t xd21 = v1->x - v2->x;
		scalar_t zd32 = v2->z - v3->z;
		scalar_t zd31 = v1->z - v3->z;
		scalar_t zd21 = v1->z - v2->z;

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


		std::size_t point_lights_count = scene.num_point_lights;

		scalar_t ndotl1 = 1, ndotl2 = 1, ndotl3 = 1;

		for (std::size_t i = 0; i < point_lights_count; i++)
		{
			ndotl1 *= vector3_dot (
				v1 -> normal,
				vector3_normalized (
					vector3_sub (
						scene.point_lights [i]->transform.T,
						v1 -> world_pos
					)
				)
			);
			ndotl2 *= vector3_dot (
				v2 -> normal,
				vector3_normalized (
					vector3_sub (
						scene.point_lights [i]->transform.T,
						v2 -> world_pos
					)
				)
			);
			ndotl3 *= vector3_dot (
				v3 -> normal,
				vector3_normalized (
					vector3_sub (
						scene.point_lights [i]->transform.T,
						v3 -> world_pos
					)
				)
			);
		}

		scalar_t cd32 = ndotl2 - ndotl3;
		scalar_t cd31 = ndotl1 - ndotl3;
		scalar_t cd21 = ndotl1 - ndotl2;

		scalar_t xg32 = xd32 / yd32;
		scalar_t xg21 = xd21 / yd21;
		scalar_t xg31 = xd31 / yd31;
		scalar_t zg32 = zd32 / yd32;
		scalar_t zg21 = zd21 / yd21;
		scalar_t zg31 = zd31 / yd31;
		scalar_t cg32 = cd32 / yd32;
		scalar_t cg31 = cd31 / yd31;
		scalar_t cg21 = cd21 / yd21;

# if 1

# define interpolate(a,b,g) (a + (b - a) * g)


		scalar_t s2 = xg32;
		scalar_t sz2 = zg32;
		scalar_t sc2 = cg32;

		int t, b;
		scalar_t l, r, ls, rs;
		scalar_t lz, rz, lzs, rzs;
		scalar_t lc, rc, lcs, rcs;

		t = v1->y, b = v2->y;

		l = v1->x;
		r = v1->x;
		ls = xg21;
		rs = xg31;

		lz = v1->z;
		rz = v1->z;
		lzs = zg21;
		rzs = zg31;

		lc = ndotl1;
		rc = ndotl1;
		lcs = cg21;
		rcs = cg31;


		bool swapped = false;

		if (v2->x > v3->x)
		{
			std::swap (ls, rs);
			std::swap (lzs, rzs);
			std::swap (lcs, rcs);
			swapped = true;
		}

		for (int y = t; y < b; y++)
		{
			scalar_t z = lz, zs = (rz - lz) / (r - l);
			scalar_t c = lc, cs = (rc - lc) / (r - l);
			for (int x = l; x <= r; x++)
			{
				color = clamp (c, 0, 1) * 0xFF;
				if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
					if (renderer->check_depth_buffer ({x, y, z}))
						renderer->put_pixel ({x, y, z}, {color, color, color});
				z += zs;
				c += cs;
			}
			l+=ls;
			r+=rs;
			lz+=lzs;
			rz+=rzs;
			lc+=lcs;
			rc+=rcs;
		}

		t = v2->y, b = v3->y;
		if (!swapped)
		{
			l = v2->x;
			r = interpolate (v1->x, v3->x, yd21 / yd31);
			lz = v2->z;
			rz = interpolate (v1->z, v3->z, yd21 / yd31);
			lc = ndotl2;
			rc = interpolate (ndotl1, ndotl3, yd21 / yd31);

			ls = s2;
			lzs = sz2;
			lcs = sc2;
		}
		else
		{
			r = v2->x;
			l = interpolate (v1->x, v3->x, yd21 / yd31);
			rz = v2->z;
			lz = interpolate (v1->z, v3->z, yd21 / yd31);
			rc = ndotl2;
			lc = interpolate (ndotl1, ndotl3, yd21 / yd31);

			rs = s2;
			rzs = sz2;
			rcs = sc2;
		}

		for (int y = t; y < b; y++)
		{
			scalar_t z = lz, zs = (rz - lz) / (r - l);
			scalar_t c = lc, cs = (rc - lc) / (r - l);
			for (int x = l; x <= r; x++)
			{
				color = clamp (c, 0, 1) * 0xFF;
				if (x > 0 && y > 0 && x < renderer_cw && y < renderer_ch)
					if (renderer->check_depth_buffer ({x, y, z}))
						renderer->put_pixel ({x, y, z}, {color, color, color});
				z += zs;
				c += cs;
			}
			l+=ls;
			r+=rs;
			lz+=lzs;
			rz+=rzs;
			lc+=lcs;
			rc+=rcs;
		}

# undef interpolate


// drwscnln:

# else

{
scalar_t yd23 = v2->y - v3->y;
scalar_t yd31 = v3->y - v1->y;
scalar_t yd21 = v2->y - v1->y;
		scalar_t ndotla, ndotlb, ndotlc, ndotld;

		va = v1, vb = v3, vc = v1, vd = v2, yds = yd31, yde = yd21;
		ndotla = ndotl1, ndotlb = ndotl3, ndotlc = ndotl1, ndotld = ndotl2;

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

			scalar_t ndotls = interpolate (ndotla, ndotlb, sg);
			scalar_t ndotle = interpolate (ndotlc, ndotld, eg);

			if (sx > ex)
			{
				std::swap (sx, ex);
				std::swap (sz, ez);
				std::swap (ndotls, ndotle);
			} // TODO: move outside the loop

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
			//         geometry->vertices [geometry->faces [i].v1].position,
			//         vector3_add (
			//             geometry->vertices [geometry->faces [i].v2].position,
			//             geometry->vertices [geometry->faces [i].v3].position
			//         )
			//     ),
			//     1.l / 3
			// );
			//
			// n = vector3_scale (
			//     vector3_add (
			//         geometry->vertices [geometry->faces [i].v1].normal,
			//         vector3_add (
			//             geometry->vertices [geometry->faces [i].v2].normal,
			//             geometry->vertices [geometry->faces [i].v3].normal
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
				scalar_t ndotl = interpolate (ndotls, ndotle, (scalar_t) (x - sx) / (ex - sx));
				color = clamp (ndotl, 0, 1) * 0xFF;
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
			ndotlc = ndotl3;
			goto draw_scan_line;
		}
}
# endif
# ifdef DBG
		draw_line (v1->x, v1->y, v2->x, v2->y, {0xff, 0, 0});
		draw_line (v2->x, v2->y, v3->x, v3->y, {0xff, 0, 0});
		draw_line (v3->x, v3->y, v1->x, v1->y, {0xff, 0, 0});
	break;
# endif
	}

	for (int i = 0; i < geometry->num_faces; i++)
	{
		break;

		const vertex_data & v1 = vertices_projected [geometry->faces [i].v1];
		const vertex_data & v2 = vertices_projected [geometry->faces [i].v2];
		const vertex_data & v3 = vertices_projected [geometry->faces [i].v3];

		draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0, 0});
		draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0, 0});
		draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0, 0});

		// vector3 c = vector3_scale ((vector3) {
		//     vertices_projected [geometry->faces [i].v1].position.x + vertices_projected [geometry->faces [i].v2].position.x + vertices_projected [geometry->faces [i].v3].position.x,
		//     vertices_projected [geometry->faces [i].v1].position.y + vertices_projected [geometry->faces [i].v2].position.y + vertices_projected [geometry->faces [i].v3].position.y,
		//     vertices_projected [geometry->faces [i].v1].position.z + vertices_projected [geometry->faces [i].v2].position.z + vertices_projected [geometry->faces [i].v3].position.z
		// }, 1./3);
        //
		// vector3 v = project (c);
		// if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
		//     renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0, 0});
	}

	for (int i = 0; i < geometry->num_vertices; i++)
	{
		break;
		const vertex_data & v = vertices_projected [i];
		if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
			renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0xff, 0xff});
	}

	delete [] vertices_projected;
}
