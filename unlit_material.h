#pragma once

namespace RaytracingRenderer {

	class UnlitMaterial: public Material {
	public:
		float3 col;

		bool scatter(
			const Ray& r_in, const hit_record& rec, float3& attenuation, Ray& scattered
		) const override {
			attenuation = col;
			return false;
		}

		UnlitMaterial() : UnlitMaterial(float3(1, 1, 1)) {}

		UnlitMaterial(float3 col) {
			this->col = col;
		}
	};
}