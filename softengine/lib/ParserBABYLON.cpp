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
		auto m = new Mesh;
		int verticesStep = 1;
		m->name = mesh ["name"].asString ();

		switch (mesh ["uvCount"].asUInt64 ())
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

		m->geometry.create (vc / verticesStep, ic / 3);

		for (int i = 0, j = 0; i < vc; i += verticesStep, j++)
		{
			m->geometry.vertices.push_back ({
				.position = {
					mesh ["vertices"] [i].asDouble (),
					mesh ["vertices"] [i + 1].asDouble (),
					mesh ["vertices"] [i + 2].asDouble ()
				},
				.normal = {
					mesh ["vertices"] [i + 3].asDouble (),
					mesh ["vertices"] [i + 4].asDouble (),
					mesh ["vertices"] [i + 5].asDouble ()
				}
			});
		}

		for (int i = 0, j = 0; i < ic; i += 3, j++)
		{
			m->geometry.faces.push_back ({
				mesh ["indices"] [i].asUInt64 (),
				mesh ["indices"] [i + 1].asUInt64 (),
				mesh ["indices"] [i + 2].asUInt64 ()
			});
		}

		if (!matrix4_equals (transform, {0}))
			m->geometry.transform (transform);

		m->position = {
			mesh ["position"] [0].asDouble (),
			mesh ["position"] [1].asDouble (),
			mesh ["position"] [2].asDouble ()
		};

		container->addChild (m);
	}

	return container;
}
