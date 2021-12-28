#pragma once

namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	class Triangle : public Object3D, public Hittable
	{
	public:
        //v1, v2, v3 must be initialized in counter-clockwise order (v0 is bottom left, v1 is bottom right, v2 is top; this can be rotated but not flipped)
		float3 v0, v1, v2, normal;

		inline Triangle(float3 v1, float3 v2, float3 v3, shared_ptr<Material> material): Object3D(position, material) {
            this->v0 = v0;
            this->v1 = v1;
            this->v2 = v2;
			this->normal = normalize(cross((v1 - v0), (v2, v0)));
		}
		inline Triangle() : Object3D(), v0(float3(-1, -1, 0)), v1(float3(1, -1, 0)), v2(float3(0, 1, 0)), normal(normalize(cross((float3(1, -1, 0) - float3(-1, -1, 0)), (float3(0, 1, 0), float3(-1, -1, 0))))) {}
        


		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			// Assuming vectors are all normalized
            float d = dot(normal, v0);

            float t = - (dot(normal, ray.orig) + d) / dot(N, ray.dir);
            if (t < 0) return false; //triangle is behind ray

            float3 p = ray.orig + t * ray.dir;

            float area = normal.length() / 2;

            //check if ray + triangle are parallel
            float denom = dot(normal, ray.dir);
            if (abs(denom) > 0.0001f == false) {
                return false;
            }

            // Intersection found. Determine if object is occluded.
            float3 diff = v0 - ray.orig;
            float t = dot(diff, normal) / denom;

            if ((t > t_min && t < t_max) == false) {
                // Object is occluded or too far away.
                return false;
            }

            float3 C; //vector perpendicular to the triangle's plane

            float3 e0 = v1 - v0;
            float3 vp0 = P - v0;
            C = cross(e0, vp0);
            if (cross(normal, C) < 0) return false; //P is on the right side

            float3 e1 = v2 - v1;
            float3 vp1 = P - v1;
            C = cross(e1, vp1);
            if (cross(normal, C) < 0) return false; //P is on the right side

            float3 e2 = v0 - v2;
            float3 vp2 = P - v2;
            C = cross(e2, vp2);
            if (cross(normal, C) < 0) return false; //P is on the right side

            // Record the hit.
            rec.t = t;
            rec.p = ray.at(t);
            rec.normal = normal;
            rec.mat_ptr = material;

            return true;
		}
	};
}