# include "softengine/engine3d/core/Object3D.h"

void Object3D :: addChild (Object3D * obj)
{
	children.push_back (std::shared_ptr <Object3D> (obj));
}
