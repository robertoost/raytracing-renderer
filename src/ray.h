#pragma once

namespace RaytracingRenderer {
    class Material;

	class Ray
	{
    public:
        inline Ray(): orig(float3(0,0,0)), dir(float3(0,0,1)) {}
        inline Ray(const float3& origin, const float3& direction): orig(origin), dir(direction) {}

        inline float3 at(float t) const {
            return orig + t * dir;
        }

        float3 orig;
        float3 dir;
        float t = INFINITY;
	};
}