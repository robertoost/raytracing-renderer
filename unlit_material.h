#pragma once

namespace RaytracingRenderer {

	class UnlitMaterial: public Material {
	public:
		float3 col;

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = col;
		}

		UnlitMaterial() : UnlitMaterial(float3(1, 1, 1)) {}

		UnlitMaterial(float3 col): Material(DIFFUSE) {
			this->col = col;
		}
	};
}