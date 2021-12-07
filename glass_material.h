#pragma once

namespace RaytracingRenderer {

	class GlassMaterial : public Material {
	public:

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(1.f, 1.f, 1.f);
		}

		GlassMaterial() : Material(GLASS) {}
	};
}
