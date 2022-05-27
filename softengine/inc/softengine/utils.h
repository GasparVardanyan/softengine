# ifndef __SOFTENGINE_UTILS_H
# define __SOFTENGINE_UTILS_H

# include "softengine/math.h"

# ifdef __cplusplus
extern "C" {
# endif // __cplusplus



void draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, unsigned char r, unsigned char g, unsigned char b, int view_width, int view_height, void (* put_pixel) (int, int, unsigned char, unsigned char, unsigned char));



# ifdef __cplusplus
}
# endif // __cplusplus

# endif // __SOFTENGINE_UTILS_H
