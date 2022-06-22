# include <iostream>
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

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

		std::size_t vpad = _i_vertices;

		for (std::size_t i = 0; i < g->num_vertices; i++)
		{
			this->geometry->vertices [_i_vertices++] = {
				vector3_transform (g->vertices [i].position, transform),
				vector3_transform_normal (g->vertices [i].normal, transform)
			};
		}

		for (std::size_t i = 0; i < g->num_faces; i++)
		{
			this->geometry->faces [_i_faces++] = {
				g->faces [i].v1 + vpad,
				g->faces [i].v2 + vpad,
				g->faces [i].v3 + vpad
			};
		}

		_num_vertices += g->num_vertices;
		_num_faces += g->num_faces;
	}
	// else if (dynamic_cast <PointLight *> (container) != nullptr)
	// {
	//     this->point_lights [_i_point_lights++] = ((PointLight *) container);
	// }

	// TODO: Object3D type ENUMMMMMMMM

	for (auto obj : container->children)
		update (obj.get (), transform);
}

void Scene :: prepare (const Object3D * container)
{
	if (dynamic_cast <const Mesh *> (container) != nullptr)
	{
		const Geometry * g = ((const Mesh *) container)->geometry;
		_num_vertices += g->num_vertices;
		_num_faces += g->num_faces;
	}
	// else if (dynamic_cast <const PointLight *> (container) != nullptr)
	// {
	//     _num_point_lights++;
	// }

	for (auto obj : container->children)
		prepare (obj.get ());
}

void Scene :: update ()
{
	if (this->geometry)
		delete this->geometry, this->geometry = nullptr;
	// if (this->point_lights)
	//     delete [] this->point_lights;

	_num_vertices = 0;
	_num_faces = 0;
	_i_vertices = 0;
	_i_faces = 0;
	_num_point_lights = 0;
	_i_point_lights = 0;
	prepare (root_container);

	this->geometry = new Geometry (_num_vertices, _num_faces);
	// this->point_lights = new PointLight * [_num_point_lights];
	update (root_container);
}
