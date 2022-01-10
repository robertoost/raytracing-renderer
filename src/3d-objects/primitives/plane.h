#pragma once

namespace RaytracingRenderer {

	class Plane: public Object3D, public Hittable
    {
	public:
        float3 normal;

        inline Plane() : Object3D(), normal(float3(0, 1, 0)) {
            this->bounding_box = AABB((float3(-INFINITY, 0, -INFINITY)), float3(INFINITY, 0, INFINITY));
        }
        inline Plane(float3 position, float3 normal, shared_ptr<Material> material) : Object3D(position, material), normal(normal) {
            this->bounding_box = AABB((float3(-INFINITY, 0, -INFINITY)), float3(INFINITY, 0, INFINITY));
        }

        void updateAABB(AABB box) const override {
            box = AABB((float3(-INFINITY, 0, -INFINITY)), float3(INFINITY, 0, INFINITY));
        }

        void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const override {
            // plane normal
            float Dnear = INFINITY;
            float Dfar = -INFINITY;
            float d;
            vector<float3> p = { float3(INFINITY, 0, INFINITY), float3(-INFINITY, 0, -INFINITY), float3(-INFINITY, 0, INFINITY), float3(INFINITY, 0, -INFINITY)};
            for (uint32_t i = 0; i < p.size(); ++i) {
                d = dot(planeNormal, p[i]);
                if (d < dnear) dnear = d;
                if (d > dfar) dfar = d;
            }
        }

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

