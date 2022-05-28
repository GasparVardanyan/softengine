# include "softengine/engine3d/primitives/Box.h"

Box::Box ()
{
	this -> geometry.vertices = std::vector <vector3>
	{
		{-1, 1, 1},
		{1, 1, 1},
		{-1, -1, 1},
		{1, -1, 1},
		{-1, 1, -1},
		{1, 1, -1},
		{1, -1, -1},
		{-1, -1, -1}
	};

	this -> geometry.faces = std::vector <vector3>
	{
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
}
