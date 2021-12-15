#pragma once

namespace RaytracingRenderer {

	class SpecularMaterial : public Material {
	public:

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = _color;
		}

		inline float specularity() const override {
			return _specularity;
		}

		inline SpecularMaterial() : Material(SOLID) {}

		inline SpecularMaterial(float specularity) : SpecularMaterial() {
			this->_specularity = specularity; 
		}

		inline SpecularMaterial(float3 color) : Material(SOLID) {
			this->_color = color;
		}

		inline SpecularMaterial(float specularity, float3 color) : Material(SOLID) {
			this->_specularity = specularity;
			this->_color = color;
		}

	private:
		float _specularity = 1.f;
		float3 _color = float3(1.f, 1.f, 1.f);
	};
}
