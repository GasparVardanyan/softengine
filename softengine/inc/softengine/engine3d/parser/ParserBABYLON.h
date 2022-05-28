# ifndef __SOFTENGINE_PARSER_BABYLON_H
# define __SOFTENGINE_PARSER_BABYLON_H

# include <json/json.h>

# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

# include <iostream>

class ParserBABYLON
{
public:
	static Object3D * parse (const Json::Value & data)
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

			for (int i = 0; i < vc; i += verticesStep)
				m -> geometry.vertices.push_back ({
					mesh ["vertices"] [i].asDouble (),
					mesh ["vertices"] [i + 1].asDouble (),
					mesh ["vertices"] [i + 2].asDouble ()
				});

			for (int i = 0; i < ic; i += 3)
				m -> geometry.faces.push_back ({
					(scalar_t) mesh ["indices"] [i].asInt (),
					(scalar_t) mesh ["indices"] [i + 1].asInt (),
					(scalar_t) mesh ["indices"] [i + 2].asInt (),
				});

			m -> position = {
				mesh ["position"] [0].asDouble (),
				mesh ["position"] [1].asDouble (),
				mesh ["position"] [2].asDouble ()
			};

			container -> addChild (m);
		}

		return container;
	}
};

# endif // __SOFTENGINE_PARSER_BABYLON_H
