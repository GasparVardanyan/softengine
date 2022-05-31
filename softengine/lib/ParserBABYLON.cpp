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

		switch (mesh ["uvCount"].asInt ())
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

		m -> geometry.create (vc / verticesStep, ic / 3);

		for (int i = 0, j = 0; i < vc; i += verticesStep, j++)
		{
			m -> geometry.vertices [j].x = mesh ["vertices"] [i].asDouble ();
			m -> geometry.vertices [j].y = mesh ["vertices"] [i + 1].asDouble ();
			m -> geometry.vertices [j].z = mesh ["vertices"] [i + 2].asDouble ();
		}

		if (!matrix4_equals (transform, {0}))
			for (int i = 0; i < m -> geometry.num_vertices; i++)
				m -> geometry.vertices [i] = vector3_transform (
					m -> geometry.vertices [i],
					transform
				);

		for (int i = 0, j = 0; i < ic; i += 3, j++)
		{
			m -> geometry.faces [j].v1 = mesh ["indices"] [i].asInt ();
			m -> geometry.faces [j].v2 = mesh ["indices"] [i + 1].asInt ();
			m -> geometry.faces [j].v3 = mesh ["indices"] [i + 2].asInt ();
		}

		m -> position = {
			mesh ["position"] [0].asDouble (),
			mesh ["position"] [1].asDouble (),
			mesh ["position"] [2].asDouble ()
		};

		container -> addChild (m);
	}

	return container;
}
