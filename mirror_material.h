#pragma once

namespace RaytracingRenderer {

	class MirrorMaterial : public Material {
	public:

		void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = 0.5 * float3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
		}

		MirrorMaterial() : Material(MIRROR) {}
	};
}
