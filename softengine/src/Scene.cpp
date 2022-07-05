# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"



# include "softengine/engine3d/materials/TextureMaterial.h"

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

	// TODO: Object3D type ENUMMMMMMMM

	for (auto obj : container->children)
		update (obj.get (), transform);
}

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
