#pragma once

namespace RaytracingRenderer {

	class Camera
	{
	private:
		float3 horizontal;
		float3 vertical;

	public:
		float screen_dist = 10.;
		float3 screen_p0;
		float3 screen_p1;
		float3 screen_p2;
		// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1) and up direction : U = (0, 1, 0)
		float3 cameraPos = float3(0, 0, 0);
		float3 cameraFront = float3(0, 0, 1);
		float3 up = float3(0, 1, 0);

		//Camera's position matrix
		float3 cameraDirection = normalize(cameraPos - cameraFront);
		float3 cameraRight = normalize(cross(cameraDirection, up));
		float3 cameraUp = cross(cameraRight, cameraFront);
		mat4 transformMatrix = mat4().LookAt(cameraPos, cameraDirection, cameraUp);
		//Camera's pitch + yawn
		float yaw = 90.f;
		float pitch = 0.f;
		float lastX, lastY;

		bool firstMouse = true;

		Camera(float3 cameraPos, float vfov, float asp_ratio)
		{
			this->cameraPos = cameraPos;
			auto theta = vfov * (PI / 180);
			auto h = tan(theta / 2);
			auto viewport_height = 2.0 * h;
			auto viewport_width = asp_ratio * viewport_height;

			auto focal_length = 1.;

			horizontal = float3(viewport_width, 0, 0) / 2;
			vertical = float3(0, viewport_height, 0) / 2;
			updateViewport();
		}

		Camera()
		{
			this->cameraPos = float3(0, 0, 0);

			auto focal_length = 1.;
			updateViewport();
		}

		void updateCameraVectors()
		{
			cameraDirection = normalize(cameraPos - cameraFront);
			cameraRight = normalize(cross(cameraDirection, cameraUp));
			cameraUp = cross(cameraRight, cameraFront);
			transformMatrix = mat4().LookAt(cameraPos, cameraDirection, cameraUp);
		}


		// If the cameraPos point was moved, this function can update the viewport accordingly.
		void updateViewport()
		{

			// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
			float3 screen_center = cameraPos + screen_dist * cameraFront;

			// Make sure the viewport size lines up with the screen resolution.
			float aspect_ratio = (float)SCRWIDTH / (float)SCRHEIGHT;
			float viewport_height = 2.f;
			float viewport_width = aspect_ratio * viewport_height;

			// Divide by 2 to traverse half the width/height away from the center.
			float3 horizontal = float3(viewport_width, 0, 0) / 2;
			float3 vertical = float3(0, viewport_height, 0) / 2;


			// rotate the horizontal and vertical directions, along with the screen center.
			// 		 this will ensure the screen corners are in the right positions.

			transformMatrix.TransformPoint(horizontal);
			transformMatrix.TransformPoint(vertical);
			transformMatrix.TransformPoint(screen_center);

			// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)
			screen_p0 = screen_center - horizontal + vertical;
			screen_p1 = screen_center + horizontal + vertical;
			screen_p2 = screen_center - horizontal - vertical;
		}

		void keyHandler(int key)
		{
			const float cameraSpeed = 0.5f;
			switch (key)
			{
			case 87: //W
				//cout << "BUTTON PUSHED ";
				cameraPos += cameraFront * cameraSpeed;
				break;
			case 83: //S
				cameraPos -= cameraFront * cameraSpeed;
				break;
			case 65: //A
				//cout << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z;
				cameraPos -= cameraRight * cameraSpeed;
				break;
			case 68: //D
				cameraPos += cameraRight * cameraSpeed;
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

			float sensitivity = 0.3f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw -= xoffset;
			pitch -= yoffset;

			cameraFront = normalize(getDirection(yaw, pitch));

			updateCameraVectors();
			updateViewport();
		}

		float3 getDirection(float yaw, float pitch)
		{
			/*if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;*/

			yaw *= (PI / 180);
			pitch *= (PI / 180);

			float3 direction;
			direction.x = cos((yaw)) * cos((pitch));
			direction.y = sin((pitch));
			direction.z = sin((yaw)) * cos((pitch));

			return direction;
		}


	};
}
