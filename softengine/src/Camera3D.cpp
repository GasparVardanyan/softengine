# include <algorithm>
# include <utility>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

struct vertex_data
{
	union {
		vector3 projected;
		struct {
			scalar_t x, y, z;
		};
	};
	vector3 world_pos;
	vector3 normal;
	union {
		uv texture_coordinates;
		struct {
			scalar_t u, v;
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
	v = vector3_transform (v, matrix4_mul (view_transform, projector));

	v.x = (v.x + .5) * renderer_cw;
	v.y = (-v.y + .5) * renderer_ch;

	return v;
}

void Camera3D :: render (const Scene & scene)
{
	// inverse the rotation (transpose the rotation matrix), negate the position

	this->view_transform = {
		 transform.xx,  transform.yx,  transform.zx,  transform.xw,
		 transform.xy,  transform.yy,  transform.zy,  transform.yw,
		 transform.xz,  transform.yz,  transform.zz,  transform.zw,
		-transform.tx, -transform.ty, -transform.tz,  transform.tw
	};



	// frame.fill (background);
	for (int i = 0; i < renderer_cs; i++)
		depth_buffer [i] = std::numeric_limits <scalar_t> :: infinity ();

	renderer->clear_canvas ();
	// std::fill (depth_buffer.begin (), depth_buffer.end (), std::numeric_limits <scalar_t> :: infinity ());
	// std::fill (bmp, { 0xff, 0x80, 0x00 });

	const Geometry * geometry = scene.geometry;
	const Material ** materials = scene.materials;

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
			.projected = project (geometry->vertices [i].position),
			.world_pos = geometry->vertices [i].position,
			.normal = geometry->vertices [i].normal,
			.texture_coordinates = geometry->vertices [i].texture_coordinates
		};
	}

	for (int i = 0; i < geometry->num_faces; i++)
	{
		// TODO: backface culling there...

		const Material * & m = materials [geometry->faces [i].textureindex];



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
			{324.772220, 248.879590, 0.994024},
			{206.308961, 250.212014, 0.993602},
			{266.997667, 338.254473, 0.992974},
			// {40,  100},
			// {100, 100},
			// {210, 270},

			// {.x = 100, .y = 50},
			// // {.x = 250, .y = 150},
			// {.x = 50, .y = 150},
			// {.x = 50, .y = 300},

			// {.x = 413, .y = 198}, {.x = 285, .y = 199}, {.x = 360, .y = 265}
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

		scalar_t yd32 = v3->y - v2->y;
		scalar_t yd31 = v3->y - v1->y;
		scalar_t yd21 = v2->y - v1->y;
		scalar_t yg23 = yd21 / yd31;

# if 0
		scalar_t xd32 = v3->x - v2->x;
		scalar_t xd31 = v3->x - v1->x;
		scalar_t xd21 = v2->x - v1->x;
		scalar_t zd32 = v3->z - v2->z;
		scalar_t zd31 = v3->z - v1->z;
		scalar_t zd21 = v2->z - v1->z;
		scalar_t id32 = ndotl3 - ndotl2;
		scalar_t id31 = ndotl3 - ndotl1;
		scalar_t id21 = ndotl2 - ndotl1;

		scalar_t xs23 = xd32 / yd32;
		scalar_t xs12 = xd21 / yd21;
		scalar_t xs13 = xd31 / yd31;
		scalar_t zs23 = zd32 / yd32;
		scalar_t zs12 = zd21 / yd21;
		scalar_t zs13 = zd31 / yd31;
		scalar_t is23 = id32 / yd32;
		scalar_t is13 = id31 / yd31;
		scalar_t is12 = id21 / yd21;


		scalar_t lx, rx, ls, rs; int ty, by, yh;
		scalar_t tz, lz, rz, lzs, rzs;

		lx = rx = v1->x;
		ty = v1->y, by = v2->y,	tz = v1->z;
		yh = by - ty + 1;

		if (yd21)
		{
			bool v2v3 = true;
			if (xs12 < xs13)
			{
				ls = xs12, rs = xs13;

				lz = v2->z;
				rz = v3->z;
				lzs = zs12;
				rzs = zs13;
			}
			else
			{
				v2v3 = false;
				ls = xs13, rs = xs12;

				lz = v3->z;
				rz = v2->z;
				lzs = zs13;
				rzs = zs12;
			}

			for (; ty < by; ty++)
			{
				scalar_t z = lz;
				scalar_t zs = (rz - lz) / (rx - lx);
				for (scalar_t x = lx; x <= rx; x++)
				{
					if (x > 0 && ty > 0 && x < renderer_cw && ty < renderer_ch)
						renderer->put_pixel ({(int) x, ty, z}, {color, color, color});
					z += zs;
				}
				lx += ls;
				rx += rs;
				lz += lzs;
				rz += rzs;
			}

			if (v2v3)
			{
				lx = v2->x;
				// rx = interpolate (v1->x, v3->x, yg23);
				ls = xs23;

				lz = v2->z;
				// rz = interpolate (v1->z, v3->z, yg23);
				lzs = zs23;
			}
			else
			{
				rx = v2->x;
				lx = interpolate (v1->x, v3->x, yg23);
				rs = xs23;

				rz = v2->z;
				lz = interpolate (v1->z, v3->z, yg23);
				rzs = zs23;
			}
		}
		else
		{
			if (xd21 > 0)
			{
				lx = v1->x;
				rx = v2->x;
				ls = xs13;
				rs = xs23;

				lz = v1->z;
				rz = v2->z;
				lzs = zs13;
				rzs = zs23;
			}
			else
			{
				lx = v2->x;
				rx = v1->x;
				ls = xs23;
				rs = xs13;

				lz = v2->z;
				rz = v1->z;
				lzs = zs23;
				rzs = zs13;
			}
		}

		by = v3->y;

		for (; ty <= by; ty++)
		{
			scalar_t z = lz;
			scalar_t zs = (rz - lz) / (rx - lx);
			for (scalar_t x = lx; x <= rx; x++)
			{
				if (x > 0 && ty > 0 && x < renderer_cw && ty < renderer_ch)
					renderer->put_pixel ({(int) x, ty, z}, {color, color, color});
				z += zs;
			}
			lx += ls;
			rx += rs;
			lz += lzs;
			rz += rzs;
		}

# elif 0

# define interpolate(a,b,g) (a + (b - a) * g)

		scalar_t s2 = xs23;
		scalar_t sz2 = zs23;
		scalar_t sc2 = is23;

		int t, b;
		scalar_t l, r, ls, rs;
		scalar_t lz, rz, lzs, rzs;
		scalar_t lc, rc, lcs, rcs;

		t = v1->y, b = v2->y;

		l = v1->x;
		r = v1->x;
		ls = xs12;
		rs = xs13;

		lz = v1->z;
		rz = v1->z;
		lzs = zs12;
		rzs = zs13;

		lc = ndotl1;
		rc = ndotl1;
		lcs = is12;
		rcs = is13;


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
			r = interpolate (v1->x, v3->x, yg23);
			lz = v2->z;
			rz = interpolate (v1->z, v3->z, yg23);
			lc = ndotl2;
			rc = interpolate (ndotl1, ndotl3, yg23);

			ls = s2;
			lzs = sz2;
			lcs = sc2;
		}
		else
		{
			r = v2->x;
			l = interpolate (v1->x, v3->x, yg23);
			rz = v2->z;
			lz = interpolate (v1->z, v3->z, yg23);
			rc = ndotl2;
			lc = interpolate (ndotl1, ndotl3, yg23);

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

		int y;
		const vertex_data * va, * vb, * vc, * vd;
		scalar_t yds, yde;

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
			int pxi = y * renderer_cw;

			scalar_t sg = yds ? (y - va->y) / yds : 1;
			scalar_t eg = yde ? (y - vc->y) / yde : 1;

			int sx = interpolate (va->x, vb->x, sg);
			int ex = interpolate (vc->x, vd->x, eg);
			scalar_t sz = interpolate (va->z, vb->z, sg);
			scalar_t ez = interpolate (vc->z, vd->z, eg);

			scalar_t ndotls = interpolate (ndotla, ndotlb, sg);
			scalar_t ndotle = interpolate (ndotlc, ndotld, eg);

			scalar_t us = interpolate (va->u, vb->u, sg);
			scalar_t ue = interpolate (vc->u, vd->u, eg);

			scalar_t vs = interpolate (va->v, vb->v, sg);
			scalar_t ve = interpolate (vc->v, vd->v, eg);

			if (sx > ex)
			{
				std::swap (sx, ex);
				std::swap (sz, ez);
				std::swap (ndotls, ndotle);
				std::swap (us, ue);
				std::swap (vs, ve);
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

			pxi += sx;

			for (x = sx; x <= ex; x++)
			{
				z = interpolate (sz, ez, (scalar_t) (x - sx) / (ex - sx));
				scalar_t ndotl = interpolate (ndotls, ndotle, (scalar_t) (x - sx) / (ex - sx));
				scalar_t u = interpolate (us, ue, (scalar_t) (x - sx) / (ex - sx));
				scalar_t v = interpolate (vs, ve, (scalar_t) (x - sx) / (ex - sx));
				scalar_t i = clamp (ndotl, 0, 1);
				if (u < 0) u = 0; if (u > 1) u = 1;
				if (v < 0) v = 0; if (v > 1) v = 1;
				color4 c = m->map (u, v);
				c.b *= i, c.g *= i, c.r *= i;
				color = i * 0xFF;
				// if (renderer->check_depth_buffer ({x, y, z}))
				if (z < depth_buffer [pxi])
				{
					// frame.put_pixel ({x, y, z}, {color, color, color});
					renderer->put_pixel ({x, y, z}, c);
					depth_buffer [pxi] = z;
				}

				pxi++;
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

	// for (int y = 0; y < renderer_ch; y++)
	//     for (int x = 0; x < renderer_cw; x++)
	//     {
	//         auto c = frame.buffer.at <cv::Vec3b> (cv::Point (x, y));
	//         renderer->put_pixel ({x, y}, {c [2], c [1], c [0]});
	//     }

	for (int i = 0; i < geometry->num_faces; i++)
	{
		break;
# ifndef DBG
		const vertex_data & v1 = vertices_projected [geometry->faces [i].v1];
		const vertex_data & v2 = vertices_projected [geometry->faces [i].v2];
		const vertex_data & v3 = vertices_projected [geometry->faces [i].v3];

		draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0, 0});
		draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0, 0});
		draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0, 0});
# endif

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
