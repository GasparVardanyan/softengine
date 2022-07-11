# ifndef __SOFTENGINE3D_RENDERER_CVRENDERER_H
# define __SOFTENGINE3D_RENDERER_CVRENDERER_H

# include <limits>

# include <opencv2/highgui.hpp>

# include "IRenderer.h"
# include "softengine/math.h"

class CvRenderer : public IRenderer
{
protected:
	cv::Mat * canvas;
	cv::Scalar background;

public:

	CvRenderer (cv::Mat * canvas, cv::Scalar background)
		: IRenderer (canvas, & background, canvas->size ().width, canvas->size ().height),
		background (background),
		canvas (canvas)
	{}

	virtual void put_pixel (point p, color4 c)
	{
		this->canvas->at <cv::Vec3b> (cv::Point (p.x, p.y)) = {c.b, c.g, c.r};
	};

	virtual void clear_canvas ()
	{
		this->canvas->setTo (this->background);
	}
};

# endif // __SOFTENGINE3D_RENDERER_CVRENDERER_H
