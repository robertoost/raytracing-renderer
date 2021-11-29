#pragma once
class Camera
{
public:
	// Camera position: 𝐸 =(0,0,0) and view direction : 𝑉 = (0, 0, 1)
	float3 camPos = float3(0, 0, 0);
	float3 viewDir = float3(0, 0, 1);

	// Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
	float screenDist = 4.f;
	float3 screenCenter = camPos + screenDist * viewDir;

	// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)
	float3 screenP0 = screenCenter + float3(-1, -1, 0);
	float3 screenP1 = screenCenter + float3(1, -1, 0);
	float3 screenP2 = screenCenter + float3(-1, 1, 0);
};

