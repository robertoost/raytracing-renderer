#pragma once

namespace RaytracingRenderer {

	class Ray
	{
    public:
        Ray(): orig(float3(0,0,0)), dir(float3(0,0,1)) {}
        Ray(const float3& origin, const float3& direction)
            : orig(origin), dir(direction)
        {}

        float3 at(float t) const {
            return orig + t * dir;
        }

    public:
        float3 orig;
        float3 dir;
        float t = INFINITY;
	};
}

