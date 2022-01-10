#pragma once

namespace RaytracingRenderer {

	class AABB {
	public:
		float3 p_min = float3(INFINITY, INFINITY, INFINITY);
		float3 p_max = float3(-INFINITY, -INFINITY, -INFINITY);

		AABB(float3 p_min, float3 p_max) {
			this->p_max = p_max;
			this->p_min = p_min;
		}
		AABB() {}

		//Makes box include point
		void addPoint(float3 p1) {
			p_min.x = min(p1.x, p_min.x);
			p_min.y = min(p1.y, p_min.y);
			p_min.z = min(p1.z, p_min.z);

			p_max.x = max(p1.x, p_max.x);
			p_max.y = max(p1.y, p_max.y);
			p_max.z = max(p1.z, p_max.z);
			//cout << p1;
		}

		bool intersectBounds(const Ray &ray, float t_min, float t_max) {
			float3 dirfrac;
			// r.dir is unit direction vector of ray
			dirfrac.x = 1.0f / ray.dir.x;
			dirfrac.y = 1.0f / ray.dir.y;
			dirfrac.z = 1.0f / ray.dir.z;
			// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
			// r.org is origin of ray
			float t1 = (p_min.x - ray.orig.x) * dirfrac.x;
			float t2 = (p_max.x - ray.orig.x) * dirfrac.x;
			float t3 = (p_min.y - ray.orig.y) * dirfrac.y;
			float t4 = (p_max.y - ray.orig.y) * dirfrac.y;
			float t5 = (p_min.z - ray.orig.z) * dirfrac.z;
			float t6 = (p_max.z - ray.orig.z) * dirfrac.z;

			float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
			float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
			float t;

			// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
			if (tmax < 0)
			{
				t = tmax;
				return false;
			}

			// if tmin > tmax, ray doesn't intersect AABB
			if (tmin > tmax)
			{
				t = tmax;
				return false;
			}

			t = tmin;
			return true;
		}
	};

}