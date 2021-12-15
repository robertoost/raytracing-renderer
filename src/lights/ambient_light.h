#pragma once

namespace RaytracingRenderer {

    class AmbientLight : public Light
    {
    public:
        inline AmbientLight(float intensity) : Light(float3(0,0,0), intensity) {}

        inline AmbientLight() : Light() {}

        // Ambient light doesn't need to consider position and normal.
        inline float3 illuminate(float3& position, float3& normal, Scene& scene) {
            return float3(1,1,1) * intensity;
        }
    };
}


