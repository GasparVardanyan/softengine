# ifndef __SOFTENGINE_RENDERER_IRENDERER_H
# define __SOFTENGINE_RENDERER_IRENDERER_H

# include <opencv2/highgui.hpp>

# include "softengine/math.h"

class IRenderer
{
public:
	virtual void draw (point p, color4 c) =0;
	virtual void clear () =0;
	const int canvas_width;
	const int canvas_height;

	IRenderer (void * scene, void * background, int canvas_width, int canvas_height)
		: canvas_width (canvas_width)
		, canvas_height (canvas_height)
	{}
};

# endif // __SOFTENGINE_RENDERER_IRENDERER_H
