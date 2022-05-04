# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <iostream>

# include "softengine/math.h"

extern "C" {

	void render ();
	extern int view_width;
	extern int view_height;

	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, {0x66, 0x44, 0x22});

	void put_pixel (int x, int y, unsigned char r, unsigned char g, unsigned char b)
	{
		scene.at <cv::Vec3b> (cv::Point (x, y)) = {b, g, r};
	}

}

int main ()
{
	while (true)
	{
		render ();
		cv::Mat view;
		cv::resize (scene, view, cv::Size (640, 480));
		cv::imshow ("softengine", view);
		scene.setTo (cv::Scalar (0x66, 0x44, 0x22));
		char key = cv::waitKey (1);
		if (key == 'q')
			break;
	}

	cv::destroyAllWindows ();
}
