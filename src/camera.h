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

 		Camera(float3 cameraPos, float fov, float aspect_ratio);
 		Camera();

 		void updateCameraVectors();

		inline void updateFOV(float fov) {
			this->fov = fov;
		}
		inline void updateAspectRatio(float aspect_ratio) {
			this->aspect_ratio = aspect_ratio;
		}

		void updateViewport();
		void keyHandler(int key);
		void mouseHandler(int x, int y);

		//Calculates new camera look vector
		float3 getDirection(float yaw, float pitch);
	};
}
