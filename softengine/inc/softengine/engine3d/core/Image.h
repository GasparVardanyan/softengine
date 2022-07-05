# ifndef __SOFTENGINE3D_CORE_IMAGE_H
# define __SOFTENGINE3D_CORE_IMAGE_H

# include <cstddef>

# include <opencv2/highgui.hpp>

# include "softengine/math.h"

class Image
{
public:
	cv::Mat buffer;			// TODO: avoid cv::Mat
	std::size_t width;
	std::size_t height;

	Image ()
		: width (0)
		, height (0)
	{}

	void create (const std::size_t & width, const std::size_t & height, const color4 & color)
	{
		buffer = cv::Mat (this->width = width, this->height = height, CV_8UC3);
		fill (color);
	}

	void fill (const color4 & color)
	{
		buffer.setTo (cv::Scalar (color.b, color.g, color.r));
	}

	void set_pixel (const point & p, const color4 & c)
	{
		buffer.at <cv::Vec3b> (cv::Point (p.x, p.y)) = {c.b, c.g, c.r};
	};

	color4 get_pixel (int x, int y) const
	{
		auto px = buffer.at <cv::Vec3b> (cv::Point (x, y));
		return { .b = px [0], .g = px [1], .r = px [2] };
	}

	void load (const char * const path)
	{
		buffer = cv::imread (path);

		height = buffer.size [0];
		width = buffer.size [1];
	}
};

# endif // __SOFTENGINE3D_CORE_IMAGE_H
