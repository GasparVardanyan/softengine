# include <memory>

# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/primitives/Box.h"
# include "softengine/engine3d/utils/CvRenderer.h"

# define PI 3.141592653589793l

const int view_width = 640;
const int view_height = 460;

int main ()
{
	cv::Scalar background (0x66, 0x44, 0x22);
	cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);

	Object3D rootContainer;
	Camera3D camera;
	camera.projector = perspective_projection (45 * PI / 180, 0.1, 100.0, (scalar_t) view_height / view_width);
	camera.renderer = std::shared_ptr <CvRenderer> (new CvRenderer (& scene, cv::Scalar (0x66, 0x44, 0x22)));

	Box * box = new Box ();
	box -> position.z = 15;
	box -> scale.x = 1.5;
	// print_matrix (box -> transform);
	rootContainer.addChild (box);

	while (true)
	{
		camera.render (& rootContainer);
		cv::imshow ("softengine", scene);
		// if (!s) scene.setTo (background);
		char key = cv::waitKey (1);
		if (key == 'q')
			break;

		box -> rotation.x += .005;
		box -> rotation.y += .005;
	}

	cv::destroyAllWindows ();
}
