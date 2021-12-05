#pragma once
#include "material.h"

namespace RaytracingRenderer {

	class BasicColorMaterial: Material {
	public:
		bool scatter(
			const Ray& r_in, const hit_record& rec, float3& attenuation, Ray& scattered
		) const {
			return true;
		}

	};
}