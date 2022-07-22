# include <chrono>
# include <cmath>
# include <fstream>
# include <memory>

# include <opencv2/opencv.hpp>
# include <opencv2/calib3d/calib3d.hpp>
# include <opencv2/calib3d/calib3d_c.h>
# include <opencv2/highgui/highgui.hpp>
# include <opencv2/imgproc/imgproc.hpp>
# include <opencv2/core/types_c.h>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/core/Object3D.h"
# include "softengine/engine3d/core/Scene.h"
# include "softengine/engine3d/lights/PointLight.h"
# include "softengine/engine3d/materials/FillMaterial.h"
# include "softengine/engine3d/materials/TextureMaterial.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/engine3d/parser/ParserBABYLON.h"
# include "softengine/engine3d/primitives/Box.h"
# include "softengine/engine3d/primitives/Plane.h"
# include "softengine/engine3d/renderer/CvRenderer.h"
# include "softengine/engine3d/renderer/ImageRenderer.h"

const int view_width = 640;
const int view_height = 460;
const scalar_t fov = 45 * M_PI / 180;
const scalar_t near_clipping = 0.1;
const scalar_t far_clipping = 10000;


class Calibrator
{
private:
	cv::Size chessboard_corners;
	std::vector <std::vector <cv::Point3f>> objpoints;
	std::vector <std::vector <cv::Point2f>> imgpoints;

	cv::Mat gray;
	std::vector <cv::Point2f> corner_pts;
	std::vector <cv::Point3f> objp;
	bool success;

public:
	Calibrator () {}
	Calibrator (int xc, int yc, std::vector <cv::Point3f> objp)
	{
		begin (xc, yc, objp);
	}

	void begin (int xc, int yc, std::vector <cv::Point3f> objp)
	{
		end ();

		this->chessboard_corners = {xc, yc};
		this->objp = objp;
	}

	void calibrate_image (cv::Mat & image, bool draw = false)
	{
		cv::cvtColor (image, gray, cv::COLOR_BGR2GRAY);

		success = cv::findChessboardCorners (
			gray,
			chessboard_corners,
			corner_pts,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE
		); // TODO: understand the flags used there

		if (success)
		{
			cv::TermCriteria criteria (CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);			// TODO: understand the flags used and the parameters
			cv::cornerSubPix (gray, corner_pts, cv::Size (11,11), cv::Size (-1, -1), criteria);

			if (draw)
				cv::drawChessboardCorners (image, chessboard_corners, corner_pts, success);

			objpoints.push_back(objp);
			imgpoints.push_back(corner_pts);
		}
	}

	void end ()
	{
		this->objpoints.clear ();
		this->imgpoints.clear ();
		this->gray = {0};
		this->corner_pts.clear ();
	}
};


int main ()
{
	/* ********** THE SCENE ********** */
		cv::Scalar background (0x66, 0x44, 0x22);
		cv::Mat scene (cv::Size (view_width, view_height), CV_8UC3, background);

		Object3D rootContainer;
		Scene scene3d (& rootContainer);

		Camera3D * camera = new Camera3D (perspective_view_projector (fov, near_clipping, far_clipping, view_width, view_height), std::shared_ptr <CvRenderer> (new CvRenderer (& scene, background)), near_clipping, far_clipping, 45 * M_PI / 180, {0x22, 0x44, 0x66});
		rootContainer.addChild (camera);

		camera->position.y = 3;
		camera->position.z = -21;



		PointLight * light = new PointLight;
		light->position = {10, 3, -30};
		rootContainer.addChild (light);


	/* ********** some objects ********** */

# if 0
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

		Box * box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (45 * M_PI / 180));
		box->setMaterial (new FillMaterial ((color4) {.hex = 0xff0000}));
		box->position.y = 3;
		// box->rotation.z = 45 * M_PI / 180;

		// box->rotation.x = -15 * M_PI / 180;
		// rootContainer.addChild (box);
		box_container->addChild (box);
		box = new Box (.25, 2, .5, MATRIX4_ROTATIONY (-45 * M_PI / 180));
		box->setMaterial (new FillMaterial ((color4) {.hex = 0xff0000}));
		box->position.y = 3;
		box_container->addChild (box);

		monkey->addChild (box_container);
# endif





	/* ********** PINHOLE CAMERAS ********** */

		int cam_mat_size = 500;
		int baseline = 2;
		scalar_t efl = fov_to_efl (fov, cam_mat_size);
		matrix4 pvp = pinhole_view_projector (efl, efl, near_clipping, far_clipping, cam_mat_size, cam_mat_size);
		// matrix4 pvp = perspective_view_projector (fov, near_clipping, far_clipping, cam_mat_size, cam_mat_size);

		Object3D * camera_container = new Object3D;
		rootContainer.addChild (camera_container);

		TextureMaterial * cam1_material = new TextureMaterial (false);
		cam1_material->raw.create (cam_mat_size, cam_mat_size, (color4) {0});

		Camera3D * cam1 = new Camera3D (pvp, std::shared_ptr <ImageRenderer> (new ImageRenderer (& cam1_material->raw, (color4) {.g = 0x80} )), near_clipping, far_clipping, 45 * M_PI / 180, {0x22, 0x44, 0x66});

		Plane * cam1_plane = new Plane (1, 1, true, MATRIX4_ROTATIONX (M_PI / 2));
		cam1_plane->setMaterial (cam1_material);
		cam1_plane->position.z = -1;
		cam1->addChild (cam1_plane);
		cam1->position.x = -baseline / 2.l;

		camera_container->addChild (cam1);

		TextureMaterial * cam2_material = new TextureMaterial (false);
		cam2_material->raw.create (cam_mat_size, cam_mat_size, (color4) {0});

		Camera3D * cam2 = new Camera3D (pvp, std::shared_ptr <ImageRenderer> (new ImageRenderer (& cam2_material->raw, (color4) {.g = 0x80} )), near_clipping, far_clipping, 45 * M_PI / 180, {0x22, 0x44, 0x66});

		Plane * cam2_plane = new Plane (1, 1, true, MATRIX4_ROTATIONX (M_PI / 2));
		cam2_plane->setMaterial (cam2_material);
		cam2_plane->position.z = -1;
		cam2->addChild (cam2_plane);
		cam2->position.x = baseline / 2.l;

		camera_container->addChild (cam2);

	/* ********** CHESSBOARD ********** */

		cv::Mat chessboard_texture_raw (1600, 1600, CV_8UC3);

		{
		for (int y = 0; y < 1600; y++)
			for (int x = 0; x < 1600; x++)
				if ((y / 200 + x / 200) % 2)
					chessboard_texture_raw.at <cv::Vec3b> (x, y) = {0x0};
				else
					chessboard_texture_raw.at <cv::Vec3b> (x, y) = {0xFF, 0xFF, 0xFF};
		}

		TextureMaterial *  chessboard_texture = new TextureMaterial;
		chessboard_texture->raw.set_raw (chessboard_texture_raw);

		Plane * chessboard = new Plane (1, 1, false, MATRIX4_ROTATIONX (-M_PI / 2));
		chessboard->setMaterial (chessboard_texture);
		chessboard->position.z = 5;

		rootContainer.addChild (chessboard);


		std::vector <cv::Point3f> objp;

		for(int i = 0; i < 7; i++)
		{
			for(int j = 0; j < 7; j++)
			{
				vector3 pt = {(j + 1) / 8.l, (i + 1) / 8.l, 0};
				pt = vector3_transform (pt, chessboard->getWorldTransform ());
				objp.push_back (cv::Point3f (pt.x, pt.y, pt.z));
			}
		}

		Calibrator calib1 (7, 7, objp), calib2 (7, 7, objp);

		int counter = 0;

		while (true)
		{
			scene3d.update ();
			scene3d.render (* cam2);

			calib2.calibrate_image (cam2_material->raw.buffer, true);

			chessboard->rotation.y -= .01;

			counter++;
			cv::imshow ("m2", cam2_material->raw.buffer);
			if (cv::waitKey (1) == '0') break;
		}

		return 0;











	int fps = 0; // loop's fps, not the camera's :d

	std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now ();

	bool pause = 0;

	bool show_pincam_canvases = 1;

	while (true)
	{
		if (!pause)
		{
			scene3d.update ();
			scene3d.render (* camera);
		}

		cv::imshow ("softengine", scene);

		// cv::imshow ("chessboard", chessboard_texture_raw);

		if (show_pincam_canvases)
		{
			cv::imshow ("m1", cam1_material->raw.buffer);
			cv::imshow ("m2", cam2_material->raw.buffer);
		}

		scalar_t move_speed = .1;
		scalar_t rotation_speed = .01;

		char key = cv::waitKey (1);

		if (key == '0')
			break;
		else if (key == '9')
			show_pincam_canvases = !show_pincam_canvases;
		else if (key == ' ')
			pause = !pause;
		else if (key == 'o')
		{
			scene3d.render (* cam1);
			scene3d.render (* cam2);
		}
		else if (!pause)
		{
			if (key == 'w')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_transform_normal (
						vector3_scale (VECTOR3_FORWARD, move_speed),
						camera->getWorldTransform ()
					)
				);
			}
			else if (key == 'a')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_transform_normal (
						vector3_scale (VECTOR3_LEFT, move_speed),
						camera->getWorldTransform ()
					)
				);
			}
			else if (key == 's')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_transform_normal (
						vector3_scale (VECTOR3_BACKWARD, move_speed),
						camera->getWorldTransform ()
					)
				);
			}
			else if (key == 'd')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_transform_normal (
						vector3_scale (VECTOR3_RIGHT, move_speed),
						camera->getWorldTransform ()
					)
				);
			}
			else if (key == 'q')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_scale (VECTOR3_DOWN, move_speed)
				);
			}
			else if (key == 'e')
			{
				camera->position = vector3_add (
					camera->position,
					vector3_scale (VECTOR3_UP, move_speed)
				);
			}
			else if (key == 'j')
				camera->rotation.y -= rotation_speed;
			else if (key == 'k')
				camera->rotation.y += rotation_speed;
			else if (key == 'h')
				camera->rotation.x -= rotation_speed;
			else if (key == 'l')
				camera->rotation.x += rotation_speed;
		}

		if (!pause)
		{
			// monkey->rotation.y += .01;
			// monkey->rotation.z += .01;
            //
			// box_container->rotation.y += .03;

			chessboard->rotation.z += .01;
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
