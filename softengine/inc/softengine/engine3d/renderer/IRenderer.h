# ifndef __SOFTENGINE_UTILS_IRENDERER_H
# define __SOFTENGINE_UTILS_IRENDERER_H

# include <opencv2/highgui.hpp>

# include "softengine/math.h"

class IRenderer
{
public:
	IRenderer (void * scene, void * background) {};
	virtual void draw (int x, int y, unsigned char r, unsigned char g, unsigned char b) =0;
	virtual void clear () =0;
	virtual int canvas_width () =0;
	virtual int canvas_height () =0;
};


# endif // __SOFTENGINE_UTILS_IRENDERER_H
