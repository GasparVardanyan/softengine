# include <math.h>
# include <stdlib.h>

# include <stdio.h>

# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xos.h>

# include "softengine/math.h"

Display * dis;
int screen;
Window win;
GC gc;

void init (void);

int main (void)
{
	init ();

	XEvent event;
	KeySym key;
	char ekeys [255];

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

	const scalar_t fov = 0.78;
	const scalar_t aspect_ratio = 480.f / 640;
	const scalar_t zfar = 10.0, znear = 0.1;

	const scalar_t _zl = zfar - znear;
	const scalar_t _znl = _zl * znear;
	const scalar_t _f = 1 / tan (fov / 2);

	/* printf ("%LG %LG %LG %LG, %LG %LG %LG\n", fov, aspect_ratio, zfar, znear, _zl, _znl, _f); */

	scalar_t x = 0, y = 0, z = 0;
	scalar_t rotx = 0, roty = 0, rotz = 0;

	while (1)
	{
		XNextEvent (dis, & event);

		if (event.type == Expose && event.xexpose.count == 0)
		{
			XClearWindow (dis, win);


			for (int i = 0; i < 8; i++)
			{
				vector3 vert = mesh_vertices [i];

				// rotation and translation
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

				matrix4 projection_matrix = MATRIX4_IDENTITY;
				/* matrix4 projection_matrix = { */
				/*     aspect_ratio * _f, 0, 0, 0, */
				/*     0, _f, 0, 0, */
				/*     0, 0, -zfar / (znear - zfar), (znear * zfar) / (znear - zfar), */
				/*     0, 0, 1, 0 */
				/* }; */

				vector3_Transform (& vert, matrix4_mul (world_matrix, projection_matrix));

				// aspect ratio
				vert.x *= aspect_ratio;

				// fov
				vert.x *= _f, vert.y *= _f;

				// z normalization
				vert.z = vert.z * _zl - _znl;

				// image space
				vector3_Mul (& vert, 1 / vert.z);

				/* printf ("%LG - %LG - %LG\n", vert.x, vert.y, vert.z); */

				// projection

				/* for (i = 0; i < 16; i++) */
				/*     printf ("%LG, ", ((scalar_t *) & world_matrix) [i]); */
				/* putchar ('\n'); */

				/* printf ("%LG - %LG - %LG\n", vert.x, vert.y, vert.z); */

				scalar_t xv = vert.x * 640 + 640 / 2;
				scalar_t yv = vert.y * 460 + 460 / 2;

				/* printf ("%" PRI_SCALAR_T " - %" PRI_SCALAR_T "\n", xv, yv); */
				XDrawRectangle (dis, win, gc, xv, yv, 5, 5);
			}
		}

		else if (event.type == KeyPress && XLookupString (& event.xkey, ekeys, 255, & key, 0) == 1)
		{
			if (ekeys [0] == 'q')
				break;
		}
	}

	XFreeGC (dis, gc);
	XDestroyWindow (dis, win);
	XCloseDisplay (dis);
}

void init (void)
{
	dis = XOpenDisplay (0);
	screen = DefaultScreen (dis);
	win = XCreateSimpleWindow (dis, DefaultRootWindow (dis), 10, 10, 640, 460, 5, 0xff0000, 0x224466);
	XSetStandardProperties (dis, win, "softengine", NULL, None, NULL, 0, NULL);
	XSelectInput (dis, win, ExposureMask | KeyPressMask);
	gc = XCreateGC (dis, win, 0, 0);
	XSetBackground (dis, gc, 0x000000);
	XClearWindow (dis, win);
	XMapRaised (dis, win);
}
