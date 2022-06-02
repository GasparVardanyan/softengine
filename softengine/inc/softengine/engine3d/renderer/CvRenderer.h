# ifndef __SOFTENGINE_RENDERER_CVRENDERER_H
# define __SOFTENGINE_RENDERER_CVRENDERER_H

# include <limits>

# include <opencv2/highgui.hpp>

# include "IRenderer.h"
# include "softengine/math.h"

class CvRenderer : public IRenderer
{
protected:
	cv::Mat * canvas;
	cv::Scalar background;

	scalar_t * depth_buffer;
public:

	CvRenderer (cv::Mat * canvas, cv::Scalar background)
		: IRenderer (canvas, & background, canvas->size ().width, canvas->size ().height),
		background (background),
		canvas (canvas)
	{
		depth_buffer = new scalar_t [canvas_height * canvas_width];
	}

	virtual void draw (point p, color4 c)
	{
		scalar_t * _z = depth_buffer + p.y * canvas_width + p.x;
		if (p._z < * _z)
		{
			this->canvas->at <cv::Vec3b> (cv::Point (p.x, p.y)) = {c.b, c.g, c.r};
			* _z = p._z;
		}
	};

	virtual void clear ()
	{
		this->canvas->setTo (this->background);

		static const int dbs = canvas_width * canvas_height;
		for (int i = 0; i < dbs; i++)
			depth_buffer [i] = std::numeric_limits <scalar_t> :: infinity ();
	}

	virtual ~CvRenderer ()
	{
		delete [] depth_buffer;
	}
};

# endif // __SOFTENGINE_RENDERER_CVRENDERER_H
