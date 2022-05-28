# ifndef __SOFTENGINE_UTILS_CVRENDERER_H
# define __SOFTENGINE_UTILS_CVRENDERER_H

# include <opencv2/highgui.hpp>

# include "softengine/math.h"
# include "softengine/engine3d/utils/IRenderer.h"

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

	virtual void draw (int x, int y, unsigned char r, unsigned char g, unsigned char b)
	{
		this -> canvas -> at <cv::Vec3b> (cv::Point (x, y)) = {b, g, r};
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


# endif // __SOFTENGINE_UTILS_CVRENDERER_H
