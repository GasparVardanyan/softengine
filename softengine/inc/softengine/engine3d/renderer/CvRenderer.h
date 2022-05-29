# ifndef __SOFTENGINE_RENDERER_CVRENDERER_H
# define __SOFTENGINE_RENDERER_CVRENDERER_H

# include <opencv2/highgui.hpp>

# include "IRenderer.h"
# include "softengine/math.h"

class CvRenderer : public IRenderer
{
public:
	cv::Mat * canvas;
	cv::Scalar background;

	CvRenderer (cv::Mat * canvas, cv::Scalar background)
		: IRenderer (canvas, & background),
		background (background)
	{
		this -> canvas = canvas;
	}

	virtual void draw (point p, color4 c)
	{
		this -> canvas -> at <cv::Vec3b> (cv::Point (p.x, p.y)) = {c.b, c.g, c.r};
	};

	virtual void clear ()
	{
		this -> canvas -> setTo (this -> background);
	}

	virtual int canvas_width ()
	{
		return canvas -> size ().width;
	}

	virtual int canvas_height ()
	{
		return canvas -> size ().height;
	}
};

# endif // __SOFTENGINE_RENDERER_CVRENDERER_H
