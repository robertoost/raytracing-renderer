#pragma once

namespace RaytracingRenderer {

	class AccelerationStructure
	{
	public:

		AccelerationStructure(){}

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec, list<shared_ptr<Hittable>>& objects) {
			bool collision = false;
			for (shared_ptr<Hittable> obj : objects) {
				if (obj->bounding_box.intersectBounds(ray, t_min, t_max)) {
					if (obj->intersect(ray, t_min, t_max, rec)) 
						collision = true;
				}
			}
			return collision;
		}
	};
}