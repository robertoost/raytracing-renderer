#pragma once

namespace RaytracingRenderer {
	class Light;

	class Scene: public Hittable
	{
	public:
		//Camera camera;
		list<shared_ptr<Light>> lights;
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
			//camera = Camera(float3(0, 0, 0), 90., ASPECT_RATIO);
			objects = list<shared_ptr<Hittable>>();
			shared_ptr<DiffuseMaterial> sphere_mat = make_shared<DiffuseMaterial>();
			shared_ptr<DiffuseMaterial> plane_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0, 0, 1)));

			objects.push_back(make_shared<Sphere>(Sphere(float3(0, 0, 4), 1.f, sphere_mat)));
			objects.push_back(make_shared<Plane>(Plane(float3(0, -1, 0), float3(0, 1, 0), plane_mat)));
		};
		Scene(list<shared_ptr<Hittable>> objects, list<shared_ptr<Light>> lights) {
			this->objects = objects;
			this->lights = lights;
		}
	};
}