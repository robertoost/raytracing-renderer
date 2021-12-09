#pragma once

namespace RaytracingRenderer {

	class GlassMaterial : public Material {
	public:

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(1.f, 1.f, 1.f);
		}

		//float specularity() const { };
		float ior() const override {
			return _ior;
		}

		GlassMaterial() : Material(GLASS) {}
		GlassMaterial(float ior) : GlassMaterial() {
			this->_ior = ior;
		}

	private:
		// Default ior of glass.
		float _ior = 1.5f;
	};
}
