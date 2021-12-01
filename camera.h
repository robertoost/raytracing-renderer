#pragma once
#include <vec3.h>

namespace RaytracingRenderer {

	class Camera
	{
	public:
		// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1)
		vec3 camPos = vec3(0, 0, 0);
		vec3 viewDir = vec3(0, 0, 1);

		// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
		float screenDist = 1.f;
		vec3 screenCenter = camPos + screenDist * viewDir;

		// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)
		vec3 screenP0 = screenCenter + vec3(-1, -1, 0);
		vec3 screenP1 = screenCenter + vec3(1, -1, 0);
		vec3 screenP2 = screenCenter + vec3(-1, 1, 0);

		Camera() {};
	};
}
