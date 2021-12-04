#pragma once
#include "camera.h"
#include "sphere.h"
#include "plane.h"

namespace RaytracingRenderer {

	class Scene
	{
	public:
		Camera camera;
		Sphere sphere;
		Plane plane;

		//list<Hittable> objects;

		Scene(): sphere(float3(0,0,4), 1.f, Material()), plane(float3(0,-1,0), float3(0,1,0), Material(float3(0,0,1))) {};
	};
}
