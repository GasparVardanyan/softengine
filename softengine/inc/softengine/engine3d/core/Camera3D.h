# ifndef __SOFTENGINE3D_CORE_CAMERA3D_H
# define __SOFTENGINE3D_CORE_CAMERA3D_H

# include <memory>

# include "softengine/engine3d/core/Image.h"
# include "softengine/engine3d/core/Geometry.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/renderer/IRenderer.h"
# include "softengine/math.h"

class CullingFrustum
{
private:
	union {
		struct {
			vector3 _near_n;
			vector3 _far_n;
			vector3 _left_n;
			vector3 _right_n;
			vector3 _top_n;
			vector3 _bottom_n;
		};
		vector3 _normals [6];
	};

public:
	union {
		struct {
			vector3 near_n;
			vector3 far_n;
			vector3 left_n;
			vector3 right_n;
			vector3 top_n;
			vector3 bottom_n;
		};
		vector3 normals [6];
	};

	CullingFrustum ()
	{
	}

	// void generate (scalar_t fov, scalar_t znear, scalar_t zfar)
	// {
	//     scalar_t f = fov / 2;
    //
	//     _near_n = VECTOR3_FORWARD;
	//     _far_n = VECTOR3_BACK;
	//     _left_n = vector3_transform_normal (VECTOR3_RIGHT, MATRIX4_ROTATIONY (f));
	//     _right_n = vector3_transform_normal (VECTOR3_LEFT, MATRIX4_ROTATIONY (-f));
	//     _top_n = vector3_transform_normal (VECTOR3_DOWN, MATRIX4_ROTATIONX (f));
	//     _bottom_n = vector3_transform_normal (VECTOR3_UP, MATRIX4_ROTATIONX (-f));
    //
	//     // TODO: where f and where -f ?
	//     // TODO: this might change fixing camera's transfom !!
	// }
    //
	// void setTransform (const matrix4 & transform)
	// {
	//     for (int i = 0; i < 6; i++)
	//         normals [i] = vector3_transform_normal (_normals [i], transform);
	// }
    //
	// bool inside (vector3 v)
	// {
	//     for (int i = 0; i < 6; i++)
	//         if (vector3_dot (normals [i], v)
	// }
};

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

	matrix4 view_transform;

	scalar_t * depth_buffer;
	std::size_t * faces_to_raster;

public:
	Image frame;
	color4 background;

	Camera3D (matrix4 projector, std::shared_ptr <IRenderer> renderer, color4 background)
		: projector (projector)
		, view_transform ({0})
		, renderer (renderer)
		, background (background)
	{
		renderer_cw = renderer->canvas_width;
		renderer_ch = renderer->canvas_height;
		renderer_cs = renderer_cw * renderer_ch;

		frame.create (renderer_cw, renderer_ch, background);

		depth_buffer = new scalar_t [renderer_cs];
		faces_to_raster = new std::size_t [renderer_cs];
	}

	void render (const Scene & scene);

	virtual ~Camera3D ()
	{
		delete [] depth_buffer;
		delete [] faces_to_raster;
	}
};

# endif // __SOFTENGINE3D_CORE_CAMERA3D_H
