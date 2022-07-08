# include <algorithm>
# include <utility>

# include "softengine/engine3d/core/Camera3D.h"
# include "softengine/engine3d/objects/Mesh.h"
# include "softengine/math.h"

// NOTE: colors are initialized with alpha=0 !!!!!

void Camera3D :: put_pixel (point p, color4 c)
{
	if (p.x > 0 && p.y > 0 && p.x < renderer_cw && p.y < renderer_ch)
		renderer->put_pixel ({p.x, p.y}, c);
}

void Camera3D :: draw_line (scalar_t x1, scalar_t y1, scalar_t x2, scalar_t y2, color4 c)
{
	// TODO: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	// TODO: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm

	scalar_t d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if (d < 4) return;

	scalar_t x = x1 + (x2 - x1) / 2;
	scalar_t y = y1 + (y2 - y1) / 2;

	put_pixel ({(int) x, (int) y, 0}, c);

	draw_line (x1, y1, x, y, c);
	draw_line (x, y, x2, y2, c);
}

// void Camera3D :: render (const Scene & scene)
// {
//
//     // frame.fill (background);
//     // std::fill (depth_buffer.begin (), depth_buffer.end (), std::numeric_limits <scalar_t> :: infinity ());
//     // std::fill (bmp, { 0xff, 0x80, 0x00 });
//
//     const Geometry * geometry = scene.geometry;
//     const Material ** materials = scene.materials;
//
//
//
//     // for (int y = 0; y < renderer_ch; y++)
//     //     for (int x = 0; x < renderer_cw; x++)
//     //     {
//     //         auto c = frame.buffer.at <cv::Vec3b> (cv::Point (x, y));
//     //         renderer->put_pixel ({x, y}, {c [2], c [1], c [0]});
//     //     }
//
//     for (int i = 0; i < geometry->num_faces; i++)
//     {
//         break;
//         const vertex_data & v1 = vertices_projected [geometry->faces [i].v1];
//         const vertex_data & v2 = vertices_projected [geometry->faces [i].v2];
//         const vertex_data & v3 = vertices_projected [geometry->faces [i].v3];
//
//         draw_line (v1.x, v1.y, v2.x, v2.y, {0, 0, 0});
//         draw_line (v2.x, v2.y, v3.x, v3.y, {0, 0, 0});
//         draw_line (v3.x, v3.y, v1.x, v1.y, {0, 0, 0});
//
//         // vector3 c = vector3_scale ((vector3) {
//         //     vertices_projected [geometry->faces [i].v1].position.x + vertices_projected [geometry->faces [i].v2].position.x + vertices_projected [geometry->faces [i].v3].position.x,
//         //     vertices_projected [geometry->faces [i].v1].position.y + vertices_projected [geometry->faces [i].v2].position.y + vertices_projected [geometry->faces [i].v3].position.y,
//         //     vertices_projected [geometry->faces [i].v1].position.z + vertices_projected [geometry->faces [i].v2].position.z + vertices_projected [geometry->faces [i].v3].position.z
//         // }, 1./3);
//         //
//         // vector3 v = project (c);
//         // if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
//         //     renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0, 0});
//     }
//
//     for (int i = 0; i < geometry->num_vertices; i++)
//     {
//         break;
//         const vertex_data & v = vertices_projected [i];
//         if (v.x > 0 && v.y > 0 && v.x < renderer_cw && v.y < renderer_ch)
//             renderer->put_pixel ({(int) v.x, (int) v.y}, {0xff, 0xff, 0xff});
//     }
// }
