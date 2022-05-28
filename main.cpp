# include <chrono>
# include <fstream>
# include <memory>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/parser/ParserBABYLON.h"
# include "softengine/engine3d/primitives/Box.h"
# include "softengine/engine3d/renderer/CvRenderer.h"

# define PI 3.141592653589793l

const int view_width = 640;
const int view_height = 460;

int main ()
{
	cv::Scalar background (0x66, 0x44, 0x22);
	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);

	Object3D rootContainer;
	Camera3D camera;
	camera.projector = perspective_projector (45 * PI / 180, 0.1, 10000.0, (scalar_t) view_height / view_width);
	camera.renderer = std::shared_ptr <CvRenderer> (new CvRenderer (& scene, cv::Scalar (0x66, 0x44, 0x22)));

	// Box * box = new Box ();
	// box -> position.z = 15;
	// box -> scale.x = 2;
	// box -> scale.y = .5;
	// rootContainer.addChild (box);

	std::ifstream monbab ("monkey.babylon", std::ifstream::binary);
	Json::Value monbin;
	monbab >> monbin;

	Object3D * monkey = ParserBABYLON::parse (monbin);

	monkey -> position.z = 7;
	monkey -> rotation.z = PI;

	rootContainer.addChild (monkey);

	int fps = 0; // loop's fps, not camera's :d

	std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now ();

	while (true)
	{
		camera.render (& rootContainer);

		cv::imshow ("softengine", scene);

		char key = cv::waitKey (1);
		if (key == 'q')
			break;

		// box -> rotation.x += .005;
		// box -> rotation.y += .005;
		monkey -> rotation.y += .005;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now ();

		if (std::chrono::duration_cast <std::chrono::nanoseconds> (end - beg).count () >= 1e9l)
		{
			beg = std::chrono::steady_clock::now ();
			std::cout << fps << std::endl;
			fps = 0;
		}
		else
			fps++;
	}

	cv::destroyAllWindows ();
}
