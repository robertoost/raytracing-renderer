#pragma once

namespace RaytracingRenderer {

	class Plane: public Object3D, public Hittable
    {
	public:
        float3 normal;

        Plane() : Object3D(), normal(float3(0, 1, 0)) {}

        Plane(float3 position, float3 normal, shared_ptr<Material> material) : Object3D(position, material), normal(normal) {}


		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {

            // Assuming vectors are all normalized
            float denom = dot(normal, ray.dir);
            if (abs(denom) > 0.0001f == false) {
                return false;
            }

            // Intersection found. Determine if object is occluded.
            float3 diff = position - ray.orig;
            float t = dot(diff, normal) / denom;

            if ((t > t_min && t < t_max) == false) {
                // Object is occluded or too far away.
                return false;
            }

            // Record the hit.
            rec.t = t;
            rec.p = ray.at(t);
            rec.normal = normal;
            rec.mat_ptr = material;

            return true;
		}

	};
}

