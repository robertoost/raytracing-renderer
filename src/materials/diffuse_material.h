#pragma once

namespace RaytracingRenderer {

	class DiffuseMaterial: public Material {
	public:
		float3 col;

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = col;
		}

		float specularity() const override { return 0.f; }

		DiffuseMaterial() : DiffuseMaterial(float3(1, 1, 1)) {}

		DiffuseMaterial(float3 col): Material(SOLID) {
			this->col = col;
		}
	};
}