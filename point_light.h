#pragma once

namespace RaytracingRenderer {

    class PointLight : public Light
    {
    public:
        PointLight(float3 position, float intensity) : Light(position, intensity) {}

        PointLight() : Light() {}

        // Ambient light doesn't need to consider position and normal.
        float3 illuminate(float3& position, float3& normal, float3& light_dir) {
            return float3(1,1,1) * dot(normal, light_dir) * intensity;
        }
    };
}


