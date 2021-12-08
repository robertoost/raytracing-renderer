#pragma once

namespace RaytracingRenderer {
    class Material;

	class Ray
	{
    public:
        Ray(): orig(float3(0,0,0)), dir(float3(0,0,1)) {
            this->mat_ptr = nullptr;
        }
        Ray(const float3& origin, const float3& direction)
            : orig(origin), dir(direction)
        {
            this->mat_ptr = nullptr;
        }
        Ray(const float3& origin, const float3& direction, shared_ptr<Material> mat_ptr)
            : orig(origin), dir(direction)
        {
            this->mat_ptr = mat_ptr;
        }

        float3 at(float t) const {
            return orig + t * dir;
        }

        float3 orig;
        float3 dir;
        float t = INFINITY;
        shared_ptr<Material> mat_ptr;
	};
}