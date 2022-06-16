# ifndef __SOFTENGINE_CORE_CAMERA3D_H
# define __SOFTENGINE_CORE_CAMERA3D_H

# include <memory>

# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/renderer/IRenderer.h"
# include "softengine/math.h"

class Camera3D : public Object3D
{
protected:
	int renderer_cw;
	int renderer_ch;
	int renderer_cs;

	matrix4 projector;
	std::shared_ptr <IRenderer> renderer;

	void put_pixel (point p, color4 c);
	void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c);
	vector3 project (vector3 v);

	scalar_t * depth_buffer;
	point * bmp;
	std::size_t * faces_to_raster;

public:
	Camera3D (matrix4 projector, std::shared_ptr <IRenderer> renderer)
		: projector (projector)
		, renderer (renderer)
	{
		renderer_cw = renderer->canvas_width;
		renderer_ch = renderer->canvas_height;
		renderer_cs = renderer_cw * renderer_ch;

		depth_buffer = new scalar_t [renderer_cs];
		faces_to_raster = new std::size_t [renderer_cs];
		bmp = new point [renderer_cs];
	}

	void render (const Scene & scene);

	virtual ~Camera3D ()
	{
		delete [] depth_buffer;
		delete [] bmp;
		delete [] faces_to_raster;
	}
};

# endif // __SOFTENGINE_CORE_CAMERA3D_H
