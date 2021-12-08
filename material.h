#pragma once

namespace RaytracingRenderer {
	struct hit_record;
	enum MAT_TYPE { SOLID, GLASS };
// diffuse = 1 - specularity
	class Material
	{
	public:
		virtual void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const = 0;

		virtual float specularity() const {
			return 0.f;
		}

		MAT_TYPE type() { return _mat_type; }

		Material(MAT_TYPE mat_type) { this->_mat_type = mat_type; }

	private:
		MAT_TYPE _mat_type;
	};
}