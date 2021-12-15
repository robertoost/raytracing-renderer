#pragma once

namespace RaytracingRenderer {
	struct hit_record;
	enum MAT_TYPE { SOLID, GLASS };
	class Material
	{
	public:
		inline virtual void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const = 0;

		inline virtual float specularity() const {
			return 0.f;
		}

		inline virtual float3 absorption() const {
			return float3(0.f, 0.f, 0.f);
		}

		inline virtual float ior() const {
			return 1.f; // ior of Air.
		}

		inline MAT_TYPE type() { return _mat_type; }

		inline Material(MAT_TYPE mat_type) { this->_mat_type = mat_type; }

	private:
		MAT_TYPE _mat_type;
	};
}