# ifndef __SOFTENGINE_CORE_CAMERA3D_H
# define __SOFTENGINE_CORE_CAMERA3D_H

# include <memory>

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/renderer/IRenderer.h"
# include "softengine/math.h"

class Camera3D : public Object3D
{
protected:
	int renderer_cw;
	int renderer_ch;

	matrix4 projector;
	std::shared_ptr <IRenderer> renderer;

	void put_pixel (point p, color4 c);
	void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c);
	void draw_scan_line (int y, const vector3 * va, const vector3 * vb, const vector3 * vc, const vector3 * vd, color4 color);
	void draw_mesh (const Geometry & geometry, const matrix4 & transform);
	vector3 project (vector3 v);

public:
	Camera3D (matrix4 projector, std::shared_ptr <IRenderer> renderer)
		: projector (projector)
		, renderer (renderer)
	{
		renderer_cw = renderer -> canvas_width ();
		renderer_ch = renderer -> canvas_height ();
	}

	void render (Object3D * container, matrix4 transform = MATRIX4_ZERO);
};

# endif // __SOFTENGINE_CORE_CAMERA3D_H
