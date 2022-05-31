# include <chrono>
# include <fstream>
# include <iostream>
# include <memory>

# include <opencv2/highgui.hpp>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/FPS.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/parser/ParserBABYLON.h"
# include "softengine/engine3d/primitives/Box.h"
# include "softengine/engine3d/renderer/CvRenderer.h"

# define PI 3.141592653589793l

const int view_width = 640;
const int view_height = 460;

# define BOX
# define MONKEY

int main ()
{
	cv::Scalar background (0x66, 0x44, 0x22);
	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);

	Object3D rootContainer;
	Camera3D camera (perspective_projector (45 * PI / 180, 0.1, 10000.0, (scalar_t) view_height / view_width), std::shared_ptr <CvRenderer> (new CvRenderer (& scene, background)));

# ifdef BOX
	Box * box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (45 * PI / 180));
	box -> position.z = 6;
	// box -> rotation.z = 45 * PI / 180;

	// box -> rotation.x = -15 * PI / 180;
	// rootContainer.addChild (box);
# endif // BOX

# ifdef MONKEY
	std::ifstream monbab ("monkey.babylon", std::ifstream::binary);
	Json::Value monbin;
	monbab >> monbin;

	Object3D * monkey = ParserBABYLON::parse (monbin, MATRIX4_SCALE (1, 1, 1));
	monkey -> position.z = 15;
	monkey -> addChild (box);
	rootContainer.addChild (monkey);
# endif // MONKEY

	int fps = 0; // loop's fps, not the camera's :d

	std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now ();

	while (true)
	{
		camera.render (& rootContainer);

		cv::imshow ("softengine", scene);

		char key = cv::waitKey (1);
		if (key == 'q')
			break;

# ifdef BOX
		box -> rotation.x += .005;
		box -> rotation.y += .005;
# endif // BOX

# ifdef MONKEY
		monkey -> rotation.y += .01;
# endif // MONKEY

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now ();

		if (std::chrono::duration_cast <std::chrono::nanoseconds> (end - beg).count () >= 1e9l)
		{
			beg = std::chrono::steady_clock::now ();
			// std::cout << fps << std::endl;
			fps = 0;
		}
		else
			fps++;
	}

	cv::destroyAllWindows ();
}
