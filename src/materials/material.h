#pragma once

namespace RaytracingRenderer {
	struct hit_record;
	enum MAT_TYPE { SOLID, GLASS, LIGHT };
	class Material
	{
	public:
		inline virtual void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const = 0;

		// Ease of use.
		inline float3 color(const Ray& r_in, const hit_record& rec) {
			float3 col(0.f, 0.f, 0.f);
			color(r_in, rec, col);
			return col;
		}

		inline virtual float specularity() const {
			return 0.f;
		}

		inline virtual float3 absorption() const {
			return float3(0.f, 0.f, 0.f);
		}

		inline virtual float ior() const {
			return 1.f; // ior of Air.
		}

		inline virtual float3 emission_color() const {
			return float3(0.f, 0.f, 0.f);
		}

		inline virtual float emission() const {
			return 0;
		}

		inline MAT_TYPE type() { return this->_mat_type; }

		inline Material(MAT_TYPE mat_type) { this->_mat_type = mat_type; }

	private:
		MAT_TYPE _mat_type;
	};
}