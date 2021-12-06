#pragma once

namespace RaytracingRenderer {

    class AmbientLight : public Light
    {
    public:

        AmbientLight(float intensity) : Light(float3(0,0,0), intensity) {}

        AmbientLight() : Light() {}

        // Ambient light doesn't need to consider position and normal.
        float3 illuminate(float3& position, float3& normal) {
            return float3(1,1,1) * intensity;
        }
    };
}


