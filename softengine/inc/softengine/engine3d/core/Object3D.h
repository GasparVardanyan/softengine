# ifndef __SOFTENGINE_CORE_OBJECT3D_H
# define __SOFTENGINE_CORE_OBJECT3D_H

# include <vector>
# include <memory>

# include "softengine/math.h"

class Object3D
{
public:
	// matrix4 transform;
	vector3 position;
	vector3 rotation;
	vector3 scale;

	std::vector <std::shared_ptr <Object3D>> children;

	void addChild (Object3D * obj);

	Object3D () :
		position ({0, 0, 0}),
		rotation ({0, 0, 0}),
		scale ({1, 1, 1})
	{};
	virtual ~Object3D () {}
};

# endif // __SOFTENGINE_CORE_OBJECT3D_H
