# ifndef __SOFTENGINE3D_RENDERER_IMAGERENDERER_H
# define __SOFTENGINE3D_RENDERER_IMAGERENDERER_H

# include <limits>

# include "IRenderer.h"
# include "softengine/math.h"

class ImageRenderer : public IRenderer
{
protected:
	Image * canvas;
	color4 background;

public:
	ImageRenderer (Image * canvas, color4 background)
		: IRenderer (canvas, & background, 0, 0),
		background (background),
		canvas (canvas)
	{}

	virtual void put_pixel (point p, color4 c)
	{
		canvas->set_pixel (p, c);
	};

	virtual void clear_canvas ()
	{
		canvas->fill (this->background);
	}
};

# endif // __SOFTENGINE3D_RENDERER_IMAGERENDERER_H
