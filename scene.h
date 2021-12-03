#pragma once
#include "camera.h"
#include "sphere.h"

namespace RaytracingRenderer {

	class Scene
	{
	public:
		Camera camera;
		Sphere sphere;
		Scene(): sphere(float3(0,0,10), 4.f) {};
	};
}
