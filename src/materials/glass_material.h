#pragma once

namespace RaytracingRenderer {

	class GlassMaterial : public Material {
	public:

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(1.f, 1.f, 1.f);
		}

		//float specularity() const { };
		inline float ior() const override {
			return _ior;
		}

		inline float3 absorption() const override {
			return _absorption;
		}

		inline GlassMaterial(): Material(GLASS) {
			this->_ior = 1.52f;
			this->_absorption = float3(0.2f, 0.2f, 0.2f);

		}
		inline GlassMaterial(float ior, float3 absorption) : Material(GLASS) {
			this->_ior = ior;
			this->_absorption = absorption;
		}

	private:
		// Default ior of glass.
		float _ior;
		float3 _absorption;
	};
}
