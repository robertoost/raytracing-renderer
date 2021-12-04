#pragma once
#include "object3d.h"
#include "hittable.h"
namespace RaytracingRenderer {

	class Plane: public Object3D, public Hittable
    {
	public:
        float3 normal;

        Plane() : Object3D(), normal(float3(0, 1, 0)) {}

        Plane(float3 position, float3 normal, Material material) : Object3D(position, material), normal(normal) {}


		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {

            // assuming vectors are all normalized
            float denom = dot(normal, ray.dir);
            if (denom > 1e-6) {
                float3 diff = position - ray.orig;
                float t = dot(diff, normal) / denom;
                if (t >= 0) {
                    cout << "PLANE HIT";
                    return (t >= 0);
                }
            }

            return false;
		}

	};
}

