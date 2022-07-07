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

	this->forward = vector3_transform_normal (VECTOR3_FORWARD, this->view_transform);



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

		// vector3 face_normal_x3 =
		//     vector3_add (
		//         geometry->vertices [geometry->faces [i].v1].normal,
		//         vector3_add (
		//             geometry->vertices [geometry->faces [i].v2].normal,
		//             geometry->vertices [geometry->faces [i].v3].normal
		//         )
		//     )
		// ;
        //
		// if (vector3_dot (forward, face_normal_x3) > 0) continue;

		const Material * & m = materials [geometry->faces [i].textureindex];

		unsigned char color = (0.25f + (i % geometry->num_faces) * 0.75f / geometry->num_faces) * 0xFF;

		// Current face vertices projected
		const vertex_data * v1 = vertices_projected + geometry->faces [i].v1;
		const vertex_data * v2 = vertices_projected + geometry->faces [i].v2;
		const vertex_data * v3 = vertices_projected + geometry->faces [i].v3;

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

# if 0

{
		scalar_t z = 1;

		/*
		 * Variable names:
		 *
		 * [a-z][a-z0-9].*w - normal valuse
		 * [a-z][a-z0-9].*p - modified for projection:
		 *                    x and y drop the fraction part,
		 *                    alse we have to deal with z, i, u and v
		 */

		int x1p, x2p, x3p;
		int y1p, y2p, y3p;
		scalar_t z1w, z2w, z3w;

		x1p = v1->x, x2p = v2->x, x3p = v3->x;
		y1p = v1->y, y2p = v2->y, y3p = v3->y;
		z1w = v1->z, z2w = v2->z, z3w = v3->z;

		scalar_t xd32w = v3->x - v2->x;
		scalar_t xd31w = v3->x - v1->x;
		scalar_t xd21w = v2->x - v1->x;
		scalar_t yd32w = v3->y - v2->y;
		scalar_t yd31w = v3->y - v1->y;
		scalar_t yd21w = v2->y - v1->y;
		scalar_t zd32w = v3->z - v2->z;
		scalar_t zd31w = v3->z - v1->z;
		scalar_t zd21w = v2->z - v1->z;
		scalar_t id32w = ndotl3 - ndotl2;
		scalar_t id31w = ndotl3 - ndotl1;
		scalar_t id21w = ndotl2 - ndotl1;
		scalar_t ud32w = v3->u - v2->u;
		scalar_t ud31w = v3->u - v1->u;
		scalar_t ud21w = v2->u - v1->u;
		scalar_t vd32w = v3->v - v2->v;
		scalar_t vd31w = v3->v - v1->v;
		scalar_t vd21w = v2->v - v1->v;

		int xd21p = x2p - x1p;
		int xd31p = x3p - x1p;
		int xd32p = x3p - x2p;
		int yd21p = y2p - y1p;
		int yd31p = y3p - y1p;
		int yd32p = y3p - y2p;

		scalar_t xs12p = (scalar_t) xd21p / yd21p;
		scalar_t xs13p = (scalar_t) xd31p / yd31p;
		scalar_t xs23p = (scalar_t) xd32p / yd32p;

		scalar_t zs12w = zd21w / yd21w;
		scalar_t zs13w = zd31w / yd31w;
		scalar_t zs23w = zd32w / yd32w;
		scalar_t is12w = id21w / yd21w;
		scalar_t is13w = id31w / yd31w;
		scalar_t is23w = id32w / yd32w;
		scalar_t us12w = ud21w / yd21w;
		scalar_t us13w = ud31w / yd31w;
		scalar_t us23w = ud32w / yd32w;
		scalar_t vs12w = vd21w / yd21w;
		scalar_t vs13w = vd31w / yd31w;
		scalar_t vs23w = vd32w / yd32w;

		scalar_t lx = x1p, rx = x1p;
		scalar_t ls, rs;

		scalar_t lz = v1->z, rz = v1->z;
		scalar_t lzs, rzs;
		scalar_t li = ndotl1, ri = ndotl1;
		scalar_t lis, ris;
		scalar_t lu = v1->u, ru = v1->u;
		scalar_t lus, rus;
		scalar_t lv = v1->v, rv = v1->v;
		scalar_t lvs, rvs;

		if (y1p != y2p)
		{
			if (xs12p < xs13p)
			{
				ls = xs12p, rs = xs13p;
				lzs = zs12w, rzs = zs13w;
				lis = is12w, ris = is13w;
				lus = us12w, rus = us13w;
				lvs = vs12w, rvs = vs13w;
			}
			else
			{
				ls = xs13p, rs = xs12p;
				lzs = zs13w, rzs = zs12w;
				lis = is13w, ris = is12w;
				lus = us13w, rus = us12w;
				lus = us13w, rus = us12w;
			}
		}
		else
		{
			if (x1p < x2p)
			{
				rx = x2p;
				ls = xs13p, rs = xs23p;

				rz = v2->z;
				lzs = zs13w, rzs = zs23w;
				ri = ndotl2;
				lis = is13w, ris = is23w;
				ru = v2->u;
				lus = us13w, rus = us23w;
				rv = v2->v;
				lvs = vs13w, rvs = vs23w;
			}
			else
			{
				lx = x2p;
				ls = xs23p, rs = xs13p;

				lz = v2->z;
				lzs = zs23w, rzs = zs23w;
				li = ndotl2;
				lis = is23w, ris = is23w;
				lu = v2->u;
				lus = us23w, rus = us23w;
				lv = v2->v;
				lvs = vs23w, rvs = vs23w;
			}
		}



				// color4 c = m->map (u, v);
				// c.b *= i, c.g *= i, c.r *= i;
				// color = i * 0xFF;
				// // if (renderer->check_depth_buffer ({x, y, z}))
				// if (z < depth_buffer [pxi])
				// {
				//     // frame.put_pixel ({x, y, z}, {color, color, color});
				//     renderer->put_pixel ({x, y, z}, c);
				//     depth_buffer [pxi] = z;
				// }



		for (int y = y1p, pxi = y1p * renderer_cw; y <= y3p; y++, pxi += renderer_cw)
		{
			scalar_t z = lz;
			scalar_t zs = (rz - lz) / (rx - lx);
			scalar_t i = li;
			scalar_t is = (ri - li) / (rx - lx);
			scalar_t u = lu;
			scalar_t us = (ru - lu) / (rx - lx);
			scalar_t v = lv;
			scalar_t vs = (rv - lv) / (rx - lx);

			for (int x = lx; x <= rx; x++)
			{
				if (z < depth_buffer [pxi + x])
				{
					// color = 0xFF * clamp (0, 1, i);
					color4 c = m->map (clamp (0, 1, u), clamp (0, 1, v));
					c.b *= i, c.g *= i, c.r *= i;
					renderer->put_pixel ({x, y}, c);
					depth_buffer [pxi + x] = z;
				}
				z += zs;
				i += is;
				u += us;
				v += vs;
			}

			if (y == y2p && y2p != y1p)
			{
				if (xs12p < xs13p)
				{
					ls = xs23p;
					lzs = zs23w;
					lis = is23w;
					lus = us23w;
					lvs = vs23w;
				}
				else
				{
					rs = xs23p;
					rzs = zs23w;
					ris = is23w;
					rus = us23w;
					rvs = vs23w;
				}
			}
			else
			{
				lx += ls, rx += rs;
				lz += lzs, rz += rzs;
				li += lis, ri += ris;
				lu += lus, ru += rus;
				lv += lvs, rv += rvs;
			}
		}
}

# else

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
