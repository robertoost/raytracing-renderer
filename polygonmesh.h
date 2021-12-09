#pragma once

namespace RaytracingRenderer {

	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	class PolygonMesh : public Object3D, public Hittable
	{
	public:
		float r2;
		float radius;

		PolygonMesh(float3 position, float radius, shared_ptr<Material> material) : Object3D(position, material) {
			this->radius = radius;
			this->r2 = radius * radius;
		}

		PolygonMesh() : Object3D() {
			radius = 1.f;
			r2 = 1.f;
		}

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			// If the ray is glass, check if this trace is happening inside the sphere.
			float t = 0;
			if (ray.mat_ptr.get() != nullptr && ray.mat_ptr->type() == GLASS) {
				t = intersectInside(ray, t_min, t_max, rec);
			}
			else {
				float3 C = position - ray.orig;
				t = dot(C, ray.dir);
				float3 Q = C - t * ray.dir;
				float p2 = dot(Q, Q);

				// Ray is outside radius of sphere.
				if (p2 > r2) {
					return false;
				}

				t -= sqrt(r2 - p2);
			}

			// Ray intersects with sphere.
			if (((t > t_min) && (t < t_max)) == false) {
				return false;
			}

			// Set the values of the recorded intersection.
			rec.t = t;
			rec.p = ray.at(t);
			rec.mat_ptr = material;

			float3 outward_normal = (rec.p - position) / radius;
			//rec.normal = outward_normal;
			rec.set_face_normal(ray, outward_normal);
			return true;
		}

		float intersectInside(const Ray& ray, float t_min, float t_max, hit_record& rec) const {
			float3 oc = ray.orig - position;

			// Get the parameters of the quadratic formula 
			float a = dot(ray.dir, ray.dir);
			float b = 2.f * dot(oc, ray.dir);
			float c = dot(oc, oc) - radius * radius;

			// t is the discriminant of the ray sphere intersection's quadratic formula, 
			// which we can use to find whether the ray intersects.
			float t = b * b - 4.f * a * c;

			// If t < 0, the ray misses.
			// If t == 0, the ray touches at one point but does not intersect (tangent)
			// If t > 0, the ray intersects at two points.
			if (t < 0.f) {
				return -1.f;
			}
			else return ((-b - sqrt(t)) / (2.f * a));
		}

		void getSurfaceProperties(const float3& P, const float3& I, const uint32_t& index, const float3& uv, float3& N, float3& st) const
		{
			N = normalize(P - position);
		}
	};
}