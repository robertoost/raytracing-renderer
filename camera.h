#pragma once
#include "transform.h";
namespace RaytracingRenderer {

	class Camera
	{
	public:
		float3 origin;
		float screen_dist = 1.;
		float3 screen_p0;
		float3 screen_p1;
		float3 screen_p2;
		// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1) and up direction : U = (0, 1, 0)
		float3 cam_pos = float3(0, 0, 0);
		float3 view_target = float3(0, 0, 1);
		float3 cam_dir = normalize(cam_pos - view_target);
		float3 up_dir = float3(0, 1, 0);
		//Camera's position matrix
		float3 cameraX = normalize(cross(up_dir, cam_dir));
		float3 cameraY = cross(cam_dir, cameraX);
		mat4 transformMatrix = mat4().LookAt(cam_pos, view_target, up_dir);
		//Camera's pitch + yawn
		float yaw = 90.f;
		float pitch = 0.f;
		float lastX, lastY;

		bool firstMouse = true;

		// If the origin point was moved, this function can update the viewport accordingly.
		void updateViewport() {
			
			// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
			float3 screen_center = cam_pos + screen_dist * view_target;

			// Make sure the viewport size lines up with the screen resolution.
			float aspect_ratio = (float)SCRWIDTH / (float)SCRHEIGHT;
			float viewport_height = 2.f;
			float viewport_width = aspect_ratio * viewport_height;

			// Divide by 2 to traverse half the width/height away from the center.
			float3 horizontal = float3(viewport_width, 0, 0) / 2;
			float3 vertical = float3(0, viewport_height, 0) / 2;

			// TODO: rotate the horizontal and vertical directions, along with the screen center.
			// 		 this will ensure the screen corners are in the right positions.

			transformMatrix.TransformPoint(horizontal);
			transformMatrix.TransformPoint(vertical);
			transformMatrix.TransformPoint(screen_center);

			// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)
			screen_p0 = screen_center - horizontal + vertical;
			screen_p1 = screen_center + horizontal + vertical;
			screen_p2 = screen_center - horizontal - vertical;
		}

		// TODO: Let the constuctor have an initial rotation.
		// Camera(whatever you use to change position and rotation) {
		// 	origin = position;
		// 	rotation = rotation; or something like that...
		// }

		void keyHandler(int key)
		{
			const float cameraSpeed = 0.5f;
			//Camera camera = scene.camera;
			switch (key)
			{
			case 87:
				cout << "BUTTON PUSHED ";
				origin += cameraSpeed * cam_dir;
				break;
			case 83:
				origin -= cameraSpeed * cam_dir;
				break;
			case 65:
				cout << origin.x << ", " << origin.y << ", " << origin.z;
				origin -= normalize(cross(cam_dir, cameraY)) * cameraSpeed;
				break;
			case 68:
				origin += normalize(cross(cam_dir, cameraY)) * cameraSpeed;
				break;
			}
			updateViewport();
		}

		void mouseHandler(int x, int y)
		{
			if (firstMouse) 
			{
				lastX = x;
				lastY = y;
				firstMouse = false;
			}

			float xoffset = x - lastX;
			float yoffset = y - lastY;
			lastX = x; 
			lastY = y;

			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw -= xoffset;
			pitch -= yoffset;

			view_target = normalize(getDirection(yaw, pitch));

			updateViewport();
		}

		float3 getDirection(float yaw, float pitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			yaw *= (PI / 180);
			pitch *= (PI / 180);

			float3 direction;
			direction.x = cos((yaw)) * cos((pitch));
			direction.y = sin((pitch));
			direction.z = sin((yaw)) * cos((pitch));

			return direction;
		}

		Camera(float3 position, float screen_distance) {
			origin = position;
			screen_dist = screen_distance;
			updateViewport();
		}

		Camera() : Camera(float3(0, 0, 0), 1.f) {};
	};
}
