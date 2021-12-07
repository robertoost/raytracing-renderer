#pragma once

namespace RaytracingRenderer {
	struct hit_record;
	enum MAT_TYPE { DIFFUSE, MIRROR, GLASS };

	class Material
	{
	public:
		virtual void color(
			const Ray& r_in, const hit_record& rec, float3& attenuation
		) const = 0;

		MAT_TYPE type() { return mat_type; }

		Material(MAT_TYPE mat_type) { this->mat_type = mat_type; }
	private:
		MAT_TYPE mat_type;

	};
}