# ifndef __SOFTENGINE3D_RENDERER_IRENDERER_H
# define __SOFTENGINE3D_RENDERER_IRENDERER_H

# include <opencv2/highgui.hpp>

# include "softengine/math.h"

class IRenderer
{
public:
	virtual void put_pixel (point p, color4 c) =0;
	virtual void clear_canvas () =0;

	const int canvas_width;
	const int canvas_height;

	IRenderer (void * scene, void * background, int canvas_width, int canvas_height)
		: canvas_width (canvas_width)
		, canvas_height (canvas_height)
	{
	}
};

# endif // __SOFTENGINE3D_RENDERER_IRENDERER_H
