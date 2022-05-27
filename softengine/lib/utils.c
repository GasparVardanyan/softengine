# include "softengine/utils.h"
# include "softengine/math.h"



void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, unsigned char r, unsigned char g, unsigned char b, int view_width, int view_height, void (* put_pixel) (int, int, unsigned char, unsigned char, unsigned char))
{
	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	if (x > 0 && y > 0 && x < view_width && y < view_height)
		put_pixel ((int) x, (int) y, r, g, b);

	draw_line (x1, y1, x, y, r, g, b, view_width, view_height, put_pixel);
	draw_line (x, y, x2, y2, r, g, b, view_width, view_height, put_pixel);
}
