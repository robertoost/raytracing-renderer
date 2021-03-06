#pragma once

namespace RaytracingRenderer {

	class CheckerboardMaterial : public Material {
	public:
		float3 odd_color;
		float3 even_color;

		inline void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const override {
			if (int(floor(rec.p.x) + floor(rec.p.z)) % 2 == 0) {
				attenuation = odd_color;
			}
			else {
				attenuation = even_color;
			}
		}

		inline CheckerboardMaterial(float3 odd_color, float3 even_color) : Material(SOLID) {
			this->odd_color = odd_color;
			this->even_color = even_color;
		}
		inline CheckerboardMaterial() : CheckerboardMaterial(float3(0, 0, 0), float3(1, 1, 1)) {}
	};
}
