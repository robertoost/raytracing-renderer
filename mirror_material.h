#pragma once

namespace RaytracingRenderer {

	class MirrorMaterial : public Material {
	public:

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = float3(0.99f, 0.99f, 0.99f);
		}

		MirrorMaterial() : Material(MIRROR) {}
	};
}
