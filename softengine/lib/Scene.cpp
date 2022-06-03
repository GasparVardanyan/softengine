# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

static std::size_t num_vertices = 0;

void Scene :: update (const Object3D * container, matrix4 transform)
{
	transform = matrix4_mul (
		MATRIX4_TRANSFORM (
			container->position,
			container->rotation,
			container->scale
		),
		transform
	);

	if (dynamic_cast <const Mesh *> (container) != nullptr)
	{
		const Geometry & g = ((const Mesh *) container)->geometry;

		for (auto v : g.vertices)
		{
			this->geometry.vertices.push_back ({
				vector3_transform (v.position, transform),
				v.normal
			});
		}

		for (auto f : g.faces)
		{
			this->geometry.faces.push_back ({
				f.v1 + num_vertices,
				f.v2 + num_vertices,
				f.v3 + num_vertices
			});
		}

		num_vertices += g.num_vertices;
	}

	for (const auto obj : container->children)
		update (obj.get (), transform);
}

void Scene :: update ()
{
	num_vertices = 0;
	this->geometry.destroy ();
	this->geometry.create ();
	update (root_container);
	this->geometry.update_sizes ();
}
