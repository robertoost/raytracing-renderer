#pragma once
#include "object3d.h"
namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	// TODO: Material.
	class Sphere : public Object3D
	{
	public:
		float r2;

		Sphere(float3 position, float radius) : Object3D(position) {
			this->r2 = radius * radius;
			cout << "Initializing Sphere!";
		}

		Sphere() : Object3D() {
			r2 = 1.f;
		}

		bool intersect(Ray ray) {
			float3 C = transform.position - ray.orig;
			float t = dot(C, ray.dir);
			float3 Q = C - t * ray.dir;
			float p2 = dot(Q, Q);

			// Ray is outside radius of sphere.
			if (p2 > r2) {
				return false;
			}

			t -= sqrt(r2 - p2);

			// Ray intersects with sphere.
			if ((t < ray.t) && (t > 0)) {
				ray.t = t;
				return true;
			}
			return false;
		}
	};
}

