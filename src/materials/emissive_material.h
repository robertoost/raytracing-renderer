#pragma once

namespace RaytracingRenderer {
	class EmissiveMaterial : public Material {
	public:
		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = _emission_color;
		}

		inline float emission() const override {
			return _emission;
		}

		inline float3 emission_color() const override {
			return _emission_color;
		}

		inline EmissiveMaterial() : Material(LIGHT) {}

		inline EmissiveMaterial(float emission) : EmissiveMaterial() {
			this->_emission = emission;
		}

		inline EmissiveMaterial(float3 color) : Material(LIGHT) {
			this->_emission_color = color;
		}

		inline EmissiveMaterial(float emission, float3 color) : Material(LIGHT) {
			this->_emission = emission;
			this->_emission_color = color;
		}

	private:
		float3 _emission_color = float3(1, 1, 1);
		float _emission = 1.f;
	};
}