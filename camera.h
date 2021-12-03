#pragma once
#include "transform.h";
#include "precomp.h";
namespace RaytracingRenderer {

	class Camera
	{
	public:
		float3 origin = float3(0, 0, 0);
		float screen_dist = 1.;
		float3 screen_p0;
		float3 screen_p1;
		float3 screen_p2;

		// If the origin point was moved, this function can update the viewport accordingly.
		void updateViewport() {
			// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1)
			float3 view_dir = float3(0, 0, 1);
			float3 up_dir = float3(0, 1, 0);

			// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
			float screen_dist = 1.f;
			float3 screen_center = origin + screen_dist * view_dir;

			// Make sure the viewport size lines up with the screen resolution.
			float aspect_ratio = SCRHEIGHT / SCRWIDTH;
			float viewport_height = 2.0;
			float viewport_width = aspect_ratio * viewport_height;

			// Divide by 2 to traverse half the width/height away from the center.
			float3 horizontal = float3(viewport_width, 0, 0) / 2;
			float3 vertical = float3(0, viewport_height, 0) / 2;

			// TODO: rotate the horizontal and vertical directions, along with the screen center.
			// 		 this will ensure the screen corners are in the right positions.

			mat4 transformMatrix = mat4().LookAt(origin, view_dir, up_dir);

			transformMatrix.TransformPoint(horizontal);
			transformMatrix.TransformPoint(vertical);
			transformMatrix.TransformPoint(screen_center);

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

		Camera(float3 position) {
			origin = position;
			updateViewport();
		}

		Camera() : Camera(float3(0, 0, 0)) {};
	};
}
