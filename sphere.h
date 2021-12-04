#pragma once
#include "object3d.h"
#include "hittable.h"

namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	// TODO: Material.
	class Sphere : public virtual Object3D, public virtual Hittable
	{
	public:
		float r2;

		Sphere(float3 position, float radius, Material material): Object3D(position, material) {
			this->r2 = radius * radius;
		}

		Sphere() : Object3D() {
			r2 = 1.f;
		}

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			float3 C = position - ray.orig;
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
				//ray->*t = t;
				return true;
			}
			return false;
		}
	};
}

