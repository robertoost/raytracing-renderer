#pragma once

namespace RaytracingRenderer {

	class CheckerboardMaterial : public Material {
	public:
		float3 odd_color;
		float3 even_color;

		bool scatter(
			const Ray& r_in, const hit_record& rec, float3& attenuation, Ray& scattered
		) const override {
			if (int(floor(rec.p.x) + floor(rec.p.z) + floor(rec.p.y)) % 2 == 0) {
				attenuation = odd_color;
			}
			else {
				attenuation = even_color;
			}
			return false;
		}


		CheckerboardMaterial(float3 &odd_color, float3 &even_color) {
			this->odd_color = odd_color;
			this->even_color = even_color;
		}
		CheckerboardMaterial() : CheckerboardMaterial(float3(0, 0, 0), float3(1, 1, 1)) {}
	};
}
// return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);