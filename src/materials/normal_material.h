#pragma once

namespace RaytracingRenderer {

	class NormalMaterial : public Material {
	public:

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			attenuation = 0.5 * float3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
		}

		inline NormalMaterial(): Material(SOLID) {}
	};
}
