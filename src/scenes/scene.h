#pragma once

namespace RaytracingRenderer {
	class Light;

	class Scene: public Hittable
	{
	public:
		//Camera camera;
		list<shared_ptr<Light>> lights;
		list<shared_ptr<Hittable>> objects;
		BVH ac;

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			
			 //AccelerationStructure ac = AccelerationStructure(objects);
			bool collision = ac.intersect(ray, t_min, t_max, rec, objects);
			//cout << "Done ";
			return collision;
			
		};

		inline Scene() {
			//camera = Camera(float3(0, 0, 0), 90., ASPECT_RATIO);
			this->objects = list<shared_ptr<Hittable>>();
			this->ac = BVH(objects);
			shared_ptr<DiffuseMaterial> sphere_mat = make_shared<DiffuseMaterial>();
			shared_ptr<DiffuseMaterial> plane_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0, 0, 1)));

			objects.push_back(make_shared<Sphere>(Sphere(float3(0, 0, 4), 1.f, sphere_mat)));
			objects.push_back(make_shared<Plane>(Plane(float3(0, -1, 0), float3(0, 1, 0), plane_mat)));
		};
		inline Scene(list<shared_ptr<Hittable>> objects, list<shared_ptr<Light>> lights) {
			this->objects = objects;
			this->lights = lights;
			this->ac = BVH(objects);
		}

		void updateAABB(AABB box) const override {
			box = AABB((float3(-INFINITY, -INFINITY, -INFINITY)), float3(INFINITY, INFINITY, INFINITY));
		}

		void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const override {
			//do something?
		}
	};
}
