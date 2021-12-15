#pragma once

namespace RaytracingRenderer {

	class DiffuseMaterial: public Material {
	public:
		float3 col;

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = col;
		}

		inline float specularity() const override { return 0.f; }

		inline DiffuseMaterial() : DiffuseMaterial(float3(1, 1, 1)) {}

		inline DiffuseMaterial(float3 col): Material(SOLID) {
			this->col = col;
		}
	};
}