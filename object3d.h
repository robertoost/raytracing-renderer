#pragma once
#include "material.h"

namespace RaytracingRenderer {
	class Object3D
	{
	public:
		float3 position;
		shared_ptr<Material> material;

		Object3D(float3 position, Material material) {
			this->position = position;
			this->material = make_shared<Material>(material);
		}
		Object3D() : position(float3(0,0,0)), material() {}

		virtual bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
		virtual void getSurfaceProperties(const float3&, const float3&, const uint32_t&, const float2&, float3&, float3&) const = 0;

	};

};