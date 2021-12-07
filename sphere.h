#pragma once

namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	// TODO: Material.
	class Sphere : public Object3D, public Hittable
	{
	public:
		float r2;
		float radius;

		Sphere(float3 position, float radius, shared_ptr<Material> material): Object3D(position, material) {
			this->radius = radius;
			this->r2 = radius * radius;
		}

		Sphere() : Object3D() {
			radius = 1.f;
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
			if (((t > t_min) && (t < t_max)) == false) {
				//ray->*t = t;
				return false;
			}


			rec.t = t;
			rec.p = ray.at(t);
			rec.mat_ptr = material;

			float3 outward_normal = (rec.p - position) / radius;
			rec.normal = outward_normal;

			return true;
		}

		void getSurfaceProperties(const float3& P, const float3& I, const uint32_t& index, const float3& uv, float3& N, float3& st) const
		{
			N = normalize(P - position);
		}
	};
}

