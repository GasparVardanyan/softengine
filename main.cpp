# include <chrono>
# include <fstream>
# include <iostream>
# include <memory>

# include <opencv2/highgui.hpp>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/parser/ParserBABYLON.h"
# include "softengine/engine3d/primitives/Box.h"
# include "softengine/engine3d/renderer/CvRenderer.h"

# define PI 3.141592653589793l

const int view_width = 640;
const int view_height = 460;

// # define BOX
# define MONKEY

int main ()
{
	cv::Scalar background (0x66, 0x44, 0x22);
	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);
	cv::Mat blank (cv::Size (view_width, view_height), CV_8UC3, background);

	Object3D rootContainer;
	Scene scene3d (& rootContainer);

	Camera3D camera (perspective_projector (45 * PI / 180, 0.1, 10000.0, (scalar_t) view_height / view_width), std::shared_ptr <CvRenderer> (new CvRenderer (& scene, background)));

# ifdef BOX
	// Box * box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (45 * PI / 180));
	Box * box = new Box (2, 2, 2);
	box->position.z = 15;

	box->rotation.x = -15 * PI / 180;
	box->rotation.y = -15 * PI / 180;

	// box->rotation.x = box->rotation.y = .69;
	rootContainer.addChild (box);
# endif // BOX

# ifdef MONKEY
	std::ifstream monbab ("monkey.babylon", std::ifstream::binary);
	Json::Value monbin;
	monbab >> monbin;

	Object3D * monkey = ParserBABYLON::parse (monbin, MATRIX4_SCALE (1, 1, 1));
	monkey->position.z = 7;
	// monkey->addChild (box);
	rootContainer.addChild (monkey);
# endif // MONKEY

	while (true)
	{
		scene3d.update ();
		camera.render (scene3d);
		cv::imshow ("softengine", scene);

		char key = cv::waitKey (1);
		if (key == 'q')
			break;

# ifdef BOX
		// box->rotation.x += .01;
		// box->rotation.y += .01;
		// std::cout << box->rotation.x << " - " << box->rotation.y << std::endl;
# endif // BOX

# ifdef MONKEY
		monkey->rotation.y += .01;
# endif // MONKEY
	}

	cv::destroyAllWindows ();
}
