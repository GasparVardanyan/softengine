# include <chrono>
# include <fstream>
# include <memory>

# include <opencv2/highgui.hpp>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Material.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/lights/PointLight.h"
# include "softengine/engine3d/materials/FillMaterial.h"
# include "softengine/engine3d/materials/TextureMaterial.h"
# include "softengine/engine3d/objects/Mesh.h"
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
	Scene scene3d (& rootContainer);

	Camera3D * camera = new Camera3D (perspective_view_projector (45 * PI / 180, 0.1, 10000.0, view_width, view_height), std::shared_ptr <CvRenderer> (new CvRenderer (& scene, background)), {0x22, 0x44, 0x66});
	rootContainer.addChild (camera);


	PointLight * light = new PointLight;
	light->position = {0, 10, -3};
	rootContainer.addChild (light);


	std::ifstream monbab ("monkey.babylon", std::ifstream::binary);
	Json::Value monbin;
	monbab >> monbin;

	Object3D * monkey = ParserBABYLON::parse (monbin, MATRIX4_SCALE (1, 1, 1));
	monkey->position.z = 15;
	// monkey->rotation.z = PI;

	TextureMaterial * material = new TextureMaterial;
	material->raw.load ("Suzanne.jpg");
	((Mesh *) monkey->children [0].get ())->setMaterial (material);

	rootContainer.addChild (monkey);

	Object3D * box_container = new Object3D;

	Box * box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (45 * PI / 180));
	box->setMaterial (new FillMaterial ((color4) {.hex = 0xff0000}));
	box->position.y = 3;
	// box->rotation.z = 45 * PI / 180;

	// box->rotation.x = -15 * PI / 180;
	// rootContainer.addChild (box);
	box_container->addChild (box);
	box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (-45 * PI / 180));
	box->setMaterial (new FillMaterial ((color4) {.hex = 0xff0000}));
	box->position.y = 3;
	box_container->addChild (box);

	monkey->addChild (box_container);

	int fps = 0; // loop's fps, not the camera's :d

	std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now ();

	bool pause = false;

	while (true)
	{
		if (!pause)
		{
			scene3d.update ();
			scene3d.render (* camera);
		}

		cv::imshow ("softengine", scene);
		// cv::imshow ("m", material->raw.buffer);

		char key = cv::waitKey (1);
		if (key == 'q')
			break;
		else if (key == ' ')
			pause = !pause;

		if (!pause)
		{
			// box->rotation.x += .01;
			// box->rotation.y += .01;

			monkey->rotation.y += .01;
			monkey->rotation.z += .01;

			box_container->rotation.y += .03;
		}

		// std::cout << monkey->rotation.y << " - " << monkey->rotation.z << std::endl;

		fps++;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now ();

		if (std::chrono::duration_cast <std::chrono::nanoseconds> (end - beg).count () >= 1e9l)
		{
			beg = end;
			std::cout << fps << std::endl;
			fps = 0;
		}
	}

	cv::destroyAllWindows ();
}
