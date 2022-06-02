# ifndef __SOFTENGINE_RENDERER_IRENDERER_H
# define __SOFTENGINE_RENDERER_IRENDERER_H

# include <opencv2/highgui.hpp>

# include "softengine/math.h"

class IRenderer
{
protected:
	scalar_t * depth_buffer;

public:
	virtual void put_pixel (point p, color4 c) =0;
	virtual void clear_canvas () =0;

	const int canvas_width;
	const int canvas_height;

	IRenderer (void * scene, void * background, int canvas_width, int canvas_height)
		: canvas_width (canvas_width)
		, canvas_height (canvas_height)
	{
		depth_buffer = new scalar_t [canvas_height * canvas_width];
	}

	bool check_depth_buffer (point p)
	{
		return depth_buffer [p.y * canvas_width + p.x] > p._z;
	}

	void clear_depth_buffer ()
	{
		static const int dbs = canvas_width * canvas_height;
		for (int i = 0; i < dbs; i++)
			depth_buffer [i] = std::numeric_limits <scalar_t> :: infinity ();
	}

	virtual ~IRenderer ()
	{
		delete [] depth_buffer;
	}
};

# endif // __SOFTENGINE_RENDERER_IRENDERER_H
