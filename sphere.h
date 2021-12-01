#pragma once
#include "vec3.h"
#include "object3d.h"
namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	// TODO: Material.
	class Sphere : Object3D
	{
	public:
		float radius;

		Sphere(point3 position, float radius) : Object3D(position) {
			this->radius = radius;
			cout << "Initializing Sphere!";
		}

		Sphere() : Object3D() {
			radius = 1.f;
		}

		bool doesRayIntersect(Ray ray) { return true; }
	};
}

