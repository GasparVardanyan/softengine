# include "softengine/math.h"

# include <math.h>
# include <stdio.h>

/* const int view_width = 160; */
/* const int view_height = 100; */
const int view_width = 640;
const int view_height = 480;

scalar_t x = 0, y = 0, z = 0;
scalar_t rotx = 0, roty = 0, rotz = 0;

extern void put_pixel (int x, int y, unsigned char r, unsigned char g, unsigned char b);

void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, unsigned char r, unsigned char g, unsigned char b);

void render (void)
{
	const scalar_t aspect_ratio = (scalar_t) view_height / view_width;
	const scalar_t fov = 0.78;
	const scalar_t zfar = 1000.0, znear = 0.1;

	const scalar_t f = 1 / tan (fov / 2);
	const scalar_t l = zfar / (zfar - znear);

	vector3 mesh_vertices [8] = {
		(vector3) {-1, 1, 1},
		(vector3) {1, 1, 1},
		(vector3) {-1, -1, 1},
		(vector3) {1, -1, 1},
		(vector3) {-1, 1, -1},
		(vector3) {1, 1, -1},
		(vector3) {1, -1, -1},
		(vector3) {-1, -1, -1}
	};


	scalar_t mesh_vertices_projected [8] [2];

	int mesh_faces [12] [3] = {
		{ 0, 1, 2 },
		{ 1, 2, 3 },
		{ 1, 3, 6 },
		{ 1, 5, 6 },
		{ 0, 1, 4 },
		{ 1, 4, 5 },
		{ 2, 3, 7 },
		{ 3, 6, 7 },
		{ 0, 2, 7 },
		{ 0, 4, 7 },
		{ 4, 5, 6 },
		{ 4, 6, 7 }
	};

	rotx += .005;
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

		matrix4 transform_matrix =
			matrix4_mul (world_matrix, view_matrix)
		;

		vert = vector3_transform (
			vert, transform_matrix
		);

		vert = vector3_transform (
			vert, projection_matrix
		);

		vert.x /= vert.z;
		vert.y /= vert.z;

		vert.x = (vert.x + .5) * view_width;
		vert.y = (vert.y + .5) * view_height;

		mesh_vertices_projected [i] [0] = vert.x;
		mesh_vertices_projected [i] [1] = vert.y;

		/* if (vert.x > 0 && vert.y > 0 && vert.x < view_width && vert.y < view_height) */
		/*     put_pixel ((int) vert.x, (int) vert.y, 0xff, 0, 0); */
	}

	for (int i = 0; i < 12; i++)
	{
		scalar_t * v1 = mesh_vertices_projected [mesh_faces [i] [0]];
		scalar_t * v2 = mesh_vertices_projected [mesh_faces [i] [1]];
		scalar_t * v3 = mesh_vertices_projected [mesh_faces [i] [2]];

		draw_line (v1 [0], v1 [1], v2 [0], v2 [1], 0, 0xff, 0);
		draw_line (v2 [0], v2 [1], v3 [0], v3 [1], 0, 0xff, 0);
		draw_line (v3 [0], v3 [1], v1 [0], v1 [1], 0, 0xff, 0);
	}
}

void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, unsigned char r, unsigned char g, unsigned char b)
{
	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	if (x > 0 && y > 0 && x < view_width && y < view_height)
		put_pixel ((int) x, (int) y, r, g, b);

	draw_line (x1, y1, x, y, r, g, b);
	draw_line (x, y, x2, y2, r, g, b);
}
