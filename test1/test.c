# include <math.h>
# include <stdio.h>

# include "softengine/math.h"
# include "softengine/utils.h"

# define PI 3.141592653589793l



const int view_width = 640;
const int view_height = 460;

extern void put_pixel (int x, int y, unsigned char r, unsigned char g, unsigned char b);



scalar_t x = 0, y = 0, z = 15;
scalar_t rotx = 0, roty = 0, rotz = 0;

matrix4 projection_matrix;
matrix4 view_matrix;


vector3 mesh_vertices [8] = {
	{-1, 1, 1},
	{1, 1, 1},
	{-1, -1, 1},
	{1, -1, 1},
	{-1, 1, -1},
	{1, 1, -1},
	{1, -1, -1},
	{-1, -1, -1}
};

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

void print_matrix (matrix4 m);

void init (void)
{
	const scalar_t a = (scalar_t) view_height / view_width;
	const scalar_t fov = 45 * PI / 180;
	const scalar_t zfar = 100.0, znear = 0.1;

	view_matrix = MATRIX4_IDENTITY;

	projection_matrix = perspective_projection (fov, znear, zfar, a);
}

int render (void)
{
	rotx += .005;
	roty += .005;

	scalar_t mesh_vertices_projected [8] [2];

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

	matrix4 transform_matrix =
		matrix4_mul (
			matrix4_mul (
				world_matrix, view_matrix
			),
			projection_matrix
		)
	;

	/* print_matrix (world_matrix); */
	/* print_matrix (projection_matrix); */
	/* print_matrix (transform_matrix); */

	for (int i = 0; i < 8; i++)
	{
		vector3 vert = mesh_vertices [i];

# if 0
		vert = vector3_transform (
			vert, matrix4_mul (world_matrix, view_matrix)
		);

		/* scalar_t zp = vert.z; */

		vert = vector3_transform (
			vert, projection_matrix
		);

		vert.x /= vert.z;
		vert.y /= vert.z;
# else
		vert = vector3_transform (vert, transform_matrix);

		/* vert.x /= vert.z; */
		/* vert.y /= vert.z; */
# endif

		vert.x = (vert.x + .5) * view_width;
		vert.y = (vert.y + .5) * view_height;

		mesh_vertices_projected [i] [0] = vert.x;
		mesh_vertices_projected [i] [1] = vert.y;

		if (vert.x > 0 && vert.y > 0 && vert.x < view_width && vert.y < view_height)
			put_pixel ((int) vert.x, (int) vert.y, 0xff, 0, 0);
	}

	for (int i = 0; i < 12; i++)
	{
		scalar_t * v1 = mesh_vertices_projected [mesh_faces [i] [0]];
		scalar_t * v2 = mesh_vertices_projected [mesh_faces [i] [1]];
		scalar_t * v3 = mesh_vertices_projected [mesh_faces [i] [2]];

		draw_line (v1 [0], v1 [1], v2 [0], v2 [1], 0, 0xff, 0, view_width, view_height, & put_pixel);
		draw_line (v2 [0], v2 [1], v3 [0], v3 [1], 0, 0xff, 0, view_width, view_height, & put_pixel);
		draw_line (v3 [0], v3 [1], v1 [0], v1 [1], 0, 0xff, 0, view_width, view_height, & put_pixel);
	}

	return 0;
}

void print_matrix (matrix4 m)
{
	for (int i = 0; i < 16; i++)
		printf ("%10LG%s", m.m [i], (i + 1) % 4 ? ", " : "\n");
	putchar ('\n');
	fflush (stdout);
}
