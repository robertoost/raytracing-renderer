#pragma once
#include "vec3.h"
namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	// TODO: Material.
	class Sphere
	{
	public:
		vec3 position;
		float radius;

		Sphere(vec3 position, float radius) {
			this->position = position;
			this->radius = radius;
			cout << "Initializing Sphere!";
		}

		Sphere() {
			position = vec3(0, 0, 0);
			radius = 1.f;
		}
	};
}

