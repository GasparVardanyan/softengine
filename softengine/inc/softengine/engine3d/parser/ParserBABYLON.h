# ifndef __SOFTENGINE_PARSER_BABYLON_H
# define __SOFTENGINE_PARSER_BABYLON_H

# include <json/json.h>

# include "softengine/engine3d/core/Object3D.h"

class ParserBABYLON
{
public:
	static Object3D * parse (const Json::Value & data, matrix4 transform = {0});
};

# endif // __SOFTENGINE_PARSER_BABYLON_H
