# ifndef __SOFTENGINE3D_CORE_OBJECT3D_H
# define __SOFTENGINE3D_CORE_OBJECT3D_H

# include <memory>
# include <string>
# include <vector>

# include "softengine/math.h"

class Object3D
{
	friend class Scene;
	friend class Camera3D;
protected:
	matrix4 transform;
public:
	std::string name;

	vector3 position;
	vector3 rotation;
	vector3 scale;

	std::vector <std::shared_ptr <Object3D>> children;

	Object3D () :
		position ({0, 0, 0}),
		rotation ({0, 0, 0}),
		scale ({1, 1, 1})
	{};

	void addChild (Object3D * obj)
	{
		children.push_back (std::shared_ptr <Object3D> (obj));
	}

	matrix4 getTransform () const
	{
		return transform;
	}

	virtual ~Object3D () {}
};

# endif // __SOFTENGINE3D_CORE_OBJECT3D_H
