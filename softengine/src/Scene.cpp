# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"



# include "softengine/engine3d/materials/TextureMaterial.h"

void Scene :: prepare (const Object3D * container)
{
	if (dynamic_cast <const Mesh *> (container) != nullptr)
	{
		const Geometry * g = ((const Mesh *) container)->geometry;
		const Material * m = ((const Mesh *) container)->material.get ();
		_num_vertices += g->num_vertices;
		_num_faces += g->num_faces;
		_i_meshes++;
	}
	else if (dynamic_cast <const PointLight *> (container) != nullptr)
	{
		num_point_lights++;
	}

	for (auto obj : container->children)
		prepare (obj.get ());
}

void Scene :: update (Object3D * container, matrix4 transform)
{
	transform = matrix4_mul (
		MATRIX4_TRANSFORM (
			container->position,
			container->rotation,
			container->scale
		),
		transform
	);

	container->transform = transform;

	if (dynamic_cast <Mesh *> (container) != nullptr)
	{
		Geometry * & g = ((Mesh *) container)->geometry;
		Material * m = ((Mesh *) container)->material.get ();

		std::size_t vpad = _i_vertices;

		for (std::size_t i = 0; i < g->num_vertices; i++)
		{
			this->geometry->vertices [_i_vertices++] = {
				.position = vector3_transform (g->vertices [i].position, transform),
				.normal = vector3_transform_normal (g->vertices [i].normal, transform),
				.texture_coordinates = g->vertices [i].texture_coordinates
			};

			// std::cout << g->vertices [i].texture_coordinates.u << " - " << g->vertices [i].texture_coordinates.v << std::endl;
		}

		for (std::size_t i = 0; i < g->num_faces; i++)
		{
			this->geometry->faces [_i_faces++] = {
				.v1 = g->faces [i].v1 + vpad,
				.v2 = g->faces [i].v2 + vpad,
				.v3 = g->faces [i].v3 + vpad,
				.textureindex = g->faces [i].textureindex + (int) _i_meshes
			};
		}

		materials [_i_meshes] = m;

		_num_vertices += g->num_vertices;
		_num_faces += g->num_faces;
		_i_meshes++;
	}
	else if (dynamic_cast <PointLight *> (container) != nullptr)
	{
		this->point_lights [_i_point_lights++] = ((const PointLight *) container);
	}
	else if (dynamic_cast <Camera3D *> (container) != nullptr)
	{
		// inverse the rotation (transpose the rotation matrix), negate the position

		Camera3D * camera = (Camera3D *) container;

		camera->view_transform = {
			 camera->transform.xx,  camera->transform.yx,  camera->transform.zx,  camera->transform.xw,
			 camera->transform.xy,  camera->transform.yy,  camera->transform.zy,  camera->transform.yw,
			 camera->transform.xz,  camera->transform.yz,  camera->transform.zz,  camera->transform.zw,
			-camera->transform.tx, -camera->transform.ty, -camera->transform.tz,  camera->transform.tw
		};

		camera->forward = vector3_transform_normal (VECTOR3_FORWARD, camera->view_transform);
	}

	// TODO: Object3D type ENUMMMMMMMM

	for (auto obj : container->children)
		update (obj.get (), transform);
}

void Scene :: update ()
{
	if (this->geometry)
		delete this->geometry;
	if (this->point_lights)
		delete [] this->point_lights;
	if (this->materials)
		delete [] this->materials;

	_i_meshes = 0;
	_num_vertices = 0;
	_num_faces = 0;
	_i_vertices = 0;
	_i_faces = 0;
	num_point_lights = 0;
	_i_point_lights = 0;

	prepare (root_container);

	this->geometry = new Geometry (_num_vertices, _num_faces);
	this->point_lights = new const PointLight * [num_point_lights];
	this->materials = new const Material * [_i_meshes];

	_i_meshes = 0;

	update (root_container);
}

void Scene::render (Camera3D & camera)
{
	for (int i = 0; i < camera.renderer_cs; i++)
		camera.depth_buffer [i] = std::numeric_limits <scalar_t> :: infinity ();

	camera.renderer->clear_canvas ();

	camera.vertices_projected = new vertex_data [geometry->num_vertices];

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
		vector3 p = vector3_transform (
			geometry->vertices [i].position,
			matrix4_mul (
				camera.view_transform,
				camera.projector
			)
		);

		p.x = (p.x + .5) * camera.renderer_cw;
		p.y = (-p.y + .5) * camera.renderer_ch;

		camera.vertices_projected [i] = (vertex_data) {
			.projected = p,
			.world_pos = geometry->vertices [i].position,
			.normal = geometry->vertices [i].normal,
			.texture_coordinates = geometry->vertices [i].texture_coordinates
		};
	}

	for (int i = 0; i < geometry->num_faces; i++)
	{
		vector3 face_normal_x3 =
			vector3_add (
				geometry->vertices [geometry->faces [i].v1].normal,
				vector3_add (
					geometry->vertices [geometry->faces [i].v2].normal,
					geometry->vertices [geometry->faces [i].v3].normal
				)
			)
		;

		if (vector3_dot (camera.forward, face_normal_x3) > 0)
		{
			continue;
		}

		const Material * & m = materials [geometry->faces [i].textureindex];

		unsigned char color = (0.25f + (i % geometry->num_faces) * 0.75f / geometry->num_faces) * 0xFF;

		// Current face vertices projected
		const vertex_data * v1 = camera.vertices_projected + geometry->faces [i].v1;
		const vertex_data * v2 = camera.vertices_projected + geometry->faces [i].v2;
		const vertex_data * v3 = camera.vertices_projected + geometry->faces [i].v3;

		// Point v1 on top, v2 on middle and v3 on bottom projected vertices
		if (v2->y < v1->y)
			std::swap (v1, v2);
		if (v3->y < v1->y)
			std::swap (v1, v3);
		if (v3->y < v2->y)
			std::swap (v2, v3);

		std::size_t point_lights_count = num_point_lights;

		scalar_t ndotl1 = 1, ndotl2 = 1, ndotl3 = 1;

		for (std::size_t i = 0; i < point_lights_count; i++)
		{
			ndotl1 *= vector3_dot (
				v1 -> normal,
				vector3_normalized (
					vector3_sub (
						point_lights [i]->transform.T,
						v1 -> world_pos
					)
				)
			);
			ndotl2 *= vector3_dot (
				v2 -> normal,
				vector3_normalized (
					vector3_sub (
						point_lights [i]->transform.T,
						v2 -> world_pos
					)
				)
			);
			ndotl3 *= vector3_dot (
				v3 -> normal,
				vector3_normalized (
					vector3_sub (
						point_lights [i]->transform.T,
						v3 -> world_pos
					)
				)
			);
		}

		int y1 = v1->y;
		int y2 = v2->y;
		int y3 = v3->y;

		if (y3 < 0 || y1 >= camera.renderer_ch)
			continue;

		if (y1 < 0)
		{
			y1 = 0;
			if (y2 < 0)
				y2 = 0;
		}
		if (y3 >= camera.renderer_ch)
		{
			y3 = camera.renderer_ch;
			if (y2 >= camera.renderer_ch)
				y2 = camera.renderer_ch;
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
			int pxi = y * camera.renderer_cw;

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
			if (ex > camera.renderer_cw - 1) ex = camera.renderer_cw - 1;

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
			// if (v.x > 0 && v.y > 0 && v.x < camera.renderer_cw && v.y < camera.renderer_ch)
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
				// if (renderer->check_camera.depth_buffer ({x, y, z}))
				if (z < camera.depth_buffer [pxi])
				{
					// frame.put_pixel ({x, y, z}, {color, color, color});
					camera.renderer->put_pixel ({x, y, z}, c);
					camera.depth_buffer [pxi] = z;
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

	delete [] camera.vertices_projected;
}
