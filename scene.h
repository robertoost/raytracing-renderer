#pragma once
#include "camera.h"
#include "sphere.h"
#include "plane.h"

namespace RaytracingRenderer {

	class Scene: public Hittable
	{
	public:
		Camera camera;

		list<shared_ptr<Hittable>> objects;
		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			bool collision = false;
			
			// Loop over all objects in the scene and try to find intersections.
			for (shared_ptr<Hittable> obj : objects) {
				const bool hit = obj->intersect(ray, t_min, t_max, rec);
				if (hit) {
					collision = true;
					t_max = rec.t;
				}
			}
			return collision;
		};

		Scene() {
			objects = list<shared_ptr<Hittable>>();
			objects.push_back(make_shared<Sphere>(Sphere(float3(0, 0, 4), 1.f, Material())));
			objects.push_back(make_shared<Plane>(Plane(float3(0, -1, 0), float3(0, 1, 0), Material(float3(0, 0, 1)))));
		};
		Scene(list<shared_ptr<Hittable>> objects) {
			this->objects = objects;
		}
	};
}
