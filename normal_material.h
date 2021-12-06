#pragma once

namespace RaytracingRenderer {

	class NormalMaterial : public Material {
	public:

		bool scatter(
			const Ray& r_in, const hit_record& rec, float3& attenuation, Ray& scattered
		) const override {
			attenuation = 0.5 * float3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
			return false;
		}

		NormalMaterial() {}
	};
}
// return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);