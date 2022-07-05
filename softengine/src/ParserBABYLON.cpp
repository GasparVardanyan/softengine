# include <json/json.h>

# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/engine3d/parser/ParserBABYLON.h"
# include "softengine/math.h"

Object3D * ParserBABYLON::parse (const Json::Value & data, matrix4 transform)
{
	Object3D * container = new Object3D;

	for (auto mesh : data ["meshes"])
	{
		Mesh * m = new Mesh;
		int verticesStep = 1;
		m->name = mesh ["name"].asString ();

		unsigned long long uvcount = mesh ["uvCount"].asUInt64 ();

		switch (uvcount)
		{
		case 0:
			verticesStep = 6;
			break;
		case 1:
			verticesStep = 8;
			break;
		case 2:
			verticesStep = 10;
			break;
		}

		int vc = mesh ["vertices"].size ();
		int ic = mesh ["indices"].size ();

		m->geometry = new Geometry (vc / verticesStep, ic / 3);

		for (int i = 0, j = 0; i < vc; i += verticesStep, j++)
		{
			m->geometry->vertices [j] = {
				.position = {
					.x = mesh ["vertices"] [i].asDouble (),
					.y = mesh ["vertices"] [i + 1].asDouble (),
					.z = mesh ["vertices"] [i + 2].asDouble ()
				},
				.normal = {
					.x = mesh ["vertices"] [i + 3].asDouble (),
					.y = mesh ["vertices"] [i + 4].asDouble (),
					.z = mesh ["vertices"] [i + 5].asDouble ()
				}
			};

			if (uvcount != 0)
				m->geometry->vertices [j].texture_coordinates = {
					.u = mesh ["vertices"] [i + 6].asDouble (),
					.v = mesh ["vertices"] [i + 7].asDouble ()
				};
		}

		for (int i = 0, j = 0; i < ic; i += 3, j++)
		{
			m->geometry->faces [j] = {
				.v1 = mesh ["indices"] [i].asUInt64 (),
				.v2 = mesh ["indices"] [i + 1].asUInt64 (),
				.v3 = mesh ["indices"] [i + 2].asUInt64 (),
				.textureindex = 0, // TODO: read this from .babylon data
			};
		}

		if (!matrix4_equals (transform, {0}))
			m->geometry->transform (transform);

		m->position = {
			.x = mesh ["position"] [0].asDouble (),
			.y = mesh ["position"] [1].asDouble (),
			.z = mesh ["position"] [2].asDouble ()
		};



		container->addChild (m);
	}

	return container;
}
