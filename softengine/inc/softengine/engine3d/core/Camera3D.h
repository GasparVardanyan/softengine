# ifndef __SOFTENGINE_CORE_CAMERA3D_H
# define __SOFTENGINE_CORE_CAMERA3D_H

# include <memory>

# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/renderer/IRenderer.h"
# include "softengine/math.h"

class Camera3D : public Object3D
{
protected:
	void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c);
	void clear_depth_buffer ();
	int renderer_cw;
	int renderer_ch;

	matrix4 projector;
	std::shared_ptr <IRenderer> renderer;

	scalar_t * depth_buffer;

public:
	Camera3D (matrix4 projector, std::shared_ptr <IRenderer> renderer)
		: projector (projector)
		, renderer (renderer)
	{
		renderer_cw = renderer -> canvas_width ();
		renderer_ch = renderer -> canvas_height ();

		depth_buffer = new scalar_t [renderer_cw * renderer_ch];
	}

	void render (Object3D * container, matrix4 transform = MATRIX4_ZERO);

	virtual ~Camera3D ()
	{
		delete [] depth_buffer;
	}
};

# endif // __SOFTENGINE_CORE_CAMERA3D_H
