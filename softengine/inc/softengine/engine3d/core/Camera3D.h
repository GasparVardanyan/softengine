# ifndef __SOFTENGINE3D_CORE_CAMERA3D_H
# define __SOFTENGINE3D_CORE_CAMERA3D_H

# include <memory>

# include "softengine/engine3d/core/Image.h"
# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/renderer/IRenderer.h"
# include "softengine/math.h"

class Camera3D : public Object3D
{
protected:
	friend class Scene;
	vertex_data * vertices_projected;

	matrix4 projector;
	std::shared_ptr <IRenderer> renderer;

	void put_pixel (point p, color4 c);
	void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c);

	matrix4 view_transform;
	vector3 forward;

	scalar_t * depth_buffer;

	scalar_t near, far, fov, hft;

public:
	int renderer_cw;
	int renderer_ch;
	int renderer_cs;
	color4 background;

	Camera3D (matrix4 view_projector, std::shared_ptr <IRenderer> renderer, scalar_t near, scalar_t far, scalar_t fov, color4 background = {0})
		: projector (view_projector)
		, view_transform ({0})
		, renderer (renderer)
		, near (near)
		, far (far)
		, fov (fov)
		, background (background)
		, forward (VECTOR3_FORWARD)
	{
		renderer_cw = renderer->canvas_width;
		renderer_ch = renderer->canvas_height;
		renderer_cs = renderer_cw * renderer_ch;

		hft = tan (fov / 2);

		depth_buffer = new scalar_t [renderer_cs];
	}

	// void render (const Scene & scene);

	virtual ~Camera3D ()
	{
		delete [] depth_buffer;
	}
};

# endif // __SOFTENGINE3D_CORE_CAMERA3D_H
