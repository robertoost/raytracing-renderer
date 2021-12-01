#pragma once
#include <vec3.h>
#include "transform.h";
namespace RaytracingRenderer {

	class Camera
	{
	public:
		point3 origin = point3(0, 0, 0);
		double screen_dist = 1.;
		point3 screen_p0;
		point3 screen_p1;
		point3 screen_p2;

		// If the origin point was moved, this function can update the viewport accordingly.
		void updateViewport() {
			// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1)
			vec3 view_dir = vec3(0, 0, 1);

			// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
			double screen_dist = 1.f;
			point3 screen_center = origin + screen_dist * view_dir;

			// Make sure the viewport size lines up with the screen resolution.
			double aspect_ratio = SCRHEIGHT / SCRWIDTH;
			double viewport_height = 2.0;
			double viewport_width = aspect_ratio * viewport_height;

			// Divide by 2 to traverse half the width/height away from the center.
			vec3 horizontal = vec3(viewport_width, 0, 0) / 2;
			vec3 vertical = vec3(0, viewport_height, 0) / 2;

			// TODO: rotate the horizontal and vertical directions, along with the screen center.
			// 		 this will ensure the screen corners are in the right positions.

			// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)
			screen_p0 = screen_center - horizontal + vertical;
			screen_p1 = screen_center + horizontal + vertical;
			screen_p2 = screen_center - horizontal - vertical;

		};

		// TODO: Let the constuctor have an initial rotation.
		// Camera(whatever you use to change position and rotation) {
		// 	origin = position;
		// 	rotation = rotation; or something like that...
		// }

		Camera(point3 position) {
			origin = position;
			updateViewport();
		}

		Camera() : Camera(point3(0, 0, 0)) {};
	};
}
