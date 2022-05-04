# include "softengine/math.h"

# include <math.h>
# include <stdio.h>

/* const int view_width = 160; */
/* const int view_height = 100; */
const int view_width = 640 / 5;
const int view_height = 480 / 5;

scalar_t x = 0, y = 0, z = 0;
scalar_t rotx = 0, roty = 0, rotz = 0;

extern void put_pixel (int x, int y, unsigned char r, unsigned char g, unsigned char b);

void render (void)
{
	const scalar_t aspect_ratio = view_width / view_height;
	const scalar_t fov = 0.78;
	const scalar_t zfar = 1000.0, znear = 0.1;

	const scalar_t f = 1 / tan (fov / 2);
	const scalar_t l = zfar / (zfar - znear);

	vector3 mesh_vertices [] = {
		(vector3) {-1, 1, 1},
		(vector3) {1, 1, 1},
		(vector3) {-1, -1, 1},
		(vector3) {-1, -1, -1},
		(vector3) {-1, 1, -1},
		(vector3) {1, 1, -1},
		(vector3) {1, -1, 1},
		(vector3) {1, -1, -1},
		(vector3) {1, -1, -1},
	};

	/* int mesh_indices [] [2] = { */
	/*     {0, 0} */
	/* }; */

	roty += .005;



	for (int i = 0; i < 8; i++)
	{
		vector3 vert = mesh_vertices [i];

		matrix4 world_matrix = matrix4_mul (
			matrix4_mul (
				MATRIX4_ROTATIONX (rotx),
				matrix4_mul (
					MATRIX4_ROTATIONY (roty),
					MATRIX4_ROTATIONZ (rotz)
				)
			),
			MATRIX4_TRANSLATION (x, y, z)
		);

		matrix4 view_matrix = MATRIX4_TRANSLATION (0, 0, 15);

		matrix4 projection_matrix = {
			aspect_ratio * f, 0, 0, 0,
			0, f, 0, 0,
			0, 0, l, -l * znear,
			0, 0, 0, 1
		};

		matrix4 transform_matrix = matrix4_mul (
			matrix4_mul (world_matrix, view_matrix),
			MATRIX4_IDENTITY
		);

		vert = vector3_transform (
			vert, transform_matrix
		);

		vert = vector3_transform (vert, projection_matrix);

		vert.x /= vert.z;
		vert.y /= vert.z;

		vert.x = (vert.x + .5) * view_width;
		vert.y = (vert.y + .5) * view_height;

		if (vert.x > 0 && vert.y > 0 && vert.x < view_width && vert.y < view_height)
			put_pixel ((int) vert.x, (int) vert.y, 0xff, 0, 0);
	}
}
