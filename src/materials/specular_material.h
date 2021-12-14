#pragma once

namespace RaytracingRenderer {

	class SpecularMaterial : public Material {
	public:

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(1.f, 1.f, 1.f);
		}

		inline float specularity() const override {
			return 1.f;
		}

		inline SpecularMaterial() : Material(SOLID) {}

		inline SpecularMaterial(float specularity) : SpecularMaterial() {
			this->_specularity = specularity; 
		}
	private:
		float _specularity = 1.f;
	};
}
