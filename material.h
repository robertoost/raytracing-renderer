#pragma once

namespace RaytracingRenderer {
	struct hit_record;

	class Material
	{
	public:
		virtual bool scatter(
			const Ray& r_in, const hit_record& rec, float3& attenuation, Ray& scattered
		) const = 0;
	};
}