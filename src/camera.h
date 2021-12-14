#pragma once

namespace RaytracingRenderer {

	class Camera
	{
	private:
		float3 horizontal;
		float3 vertical;

	public:
		float aspect_ratio = (float)SCRWIDTH / (float)SCRHEIGHT;
		float fov = 1; //values between 0.0-1.15. 0.45, 0.60, and 1 are standard
		float3 screen_p0;
		float3 screen_p1;
		float3 screen_p2;
		// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1) and up direction : U = (0, 1, 0)
		float3 cameraPos = float3(0, 0, 0);
		float3 cameraLook = float3(0, 0, -1);
		float3 up = float3(0, 1, 0);

		//Camera's view matrix
		float3 cameraRight = normalize(cross(up, cameraLook));
		float3 cameraUp = cross(cameraLook, cameraRight);

		//Camera's pitch + yaw
		float yaw = 90.f;
		float pitch = 0.f;
		float lastX, lastY;

		bool firstMouse = true;

		//Constructor with custom camera position
		Camera(float3 cameraPos, float fov, float aspect_ratio)
		{
			this->cameraPos = cameraPos;
			this->fov = fov;
			this->aspect_ratio = aspect_ratio;
			updateViewport();
		}
		//Default constructor
		Camera()
		{
			this->cameraPos = float3(0.f, 0.f, 0.f);
			updateViewport();
		}

		//Update camera view matrix with new values (used after view is moved)
		void updateCameraVectors()
		{
			//Update camera view angle
			cameraLook = normalize(getDirection(yaw, pitch));
			cameraRight = normalize(cross(up, cameraLook));
			cameraUp = cross(cameraLook, cameraRight);
			//cout << cameraPos << " " << cameraFront << " " << cameraDirection << " " << cameraRight << " ";
		}

		void updateFOV(float fov) {
			this->fov = fov;
		}
		void updateAspectRatio(float aspect_ratio) {
			this->aspect_ratio = aspect_ratio;
		}
		// If the cameraPos point was moved, this function can update the viewport accordingly.
		void updateViewport()
		{
			updateCameraVectors();

			// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
			float3 screen_center = cameraPos + fov * cameraLook;

			// Make sure the viewport size lines up with the screen resolution.
			float viewport_height = 2.f;
			float viewport_width = aspect_ratio * viewport_height;

			// Divide by 2 to traverse half the width/height away from the center.
			//Recalculate horz/vert with new camera axis
			float3 horizontal = cameraRight * viewport_width / 2;
			float3 vertical = cameraUp * viewport_height / 2;

			//cout << "Horizontal: " << horizontal << " Vertical: " << vertical << " ";

			// Screen corners : 𝑃0 = 𝐶 + (−1, −1, 0), 𝑃1 = 𝐶 + (1, −1, 0), 𝑃2 = 𝐶 + (−1, 1, 0)
			screen_p0 = screen_center - horizontal + vertical;
			screen_p1 = screen_center + horizontal + vertical;
			screen_p2 = screen_center - horizontal - vertical;
		}

		//Update camera position on WASD keypress, modulated by cameraSpeed variable
		void keyHandler(int key)
		{
			const float cameraSpeed = 0.5f;
			switch (key)
			{
			case 87: //W
				//cout << "BUTTON PUSHED ";
				cameraPos += cameraLook * cameraSpeed;
				break;
			case 83: //S
				cameraPos -= cameraLook * cameraSpeed;
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

		//Update camera view angle on mouse movement + mouse button one pressed
		void mouseHandler(int x, int y)
		{
			if (firstMouse)
			{
				lastX = x;
				lastY = y;
				firstMouse = false;
			}

			float xoffset = x - lastX;
			float yoffset = lastY - y;
			lastX = x;
			lastY = y;

			float sensitivity = 0.3f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch -= yoffset;

			//Lock camera's Y-axis to -45 - 45 degrees to prevent full rolling
			if (pitch > 45.0f)
				pitch = 45.0f;
			if (pitch < -45.0f)
				pitch = -45.0f;
			
			updateViewport();
		}

		//Calculates new camera look vector
		float3 getDirection(float yaw, float pitch)
		{			
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
