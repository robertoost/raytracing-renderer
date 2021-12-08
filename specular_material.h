#pragma once

namespace RaytracingRenderer {

	class SpecularMaterial : public Material {
	public:

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(1.f, 1.f, 1.f);
		}

		float specularity() const override {
			return 0.5f;
		}

		SpecularMaterial() : Material(SOLID) {}

		SpecularMaterial(float specularity) : SpecularMaterial() {
			this->_specularity = specularity; 
		}
	private:
		float _specularity = 1.f;
	};
}
