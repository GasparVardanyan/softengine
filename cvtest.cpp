# include <opencv2/highgui.hpp>

extern "C" {

	void init ();
	int render ();
	extern int view_width;
	extern int view_height;

	cv::Scalar background (0x66, 0x44, 0x22);

	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);

	void put_pixel (int x, int y, unsigned char r, unsigned char g, unsigned char b)
	{
		scene.at <cv::Vec3b> (cv::Point (x, y)) = {b, g, r};
	}

}

int main ()
{
	init ();

	int s = 0;

	while (true)
	{
		if (!s) s = render ();
		cv::imshow ("softengine", scene);
		if (!s) scene.setTo (background);
		char key = cv::waitKey (1);
		if (key == 'q')
			break;
	}

	cv::destroyAllWindows ();
}
