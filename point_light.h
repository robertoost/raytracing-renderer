#pragma once

namespace RaytracingRenderer {

    class PointLight : public Light
    {
    public:
        PointLight(float3 position, float intensity) : Light(position, intensity) {}

        PointLight() : Light() {}

        float3 illuminate(float3& position, float3& normal, float3& light_dir) {
            float cos_a = clamp(dot(normal, light_dir), 0.f, 1.f);

            if (cos_a == 0) {
                return float3(0, 0, 0);
            }
            return float3(1, 1, 1) * cos_a * intensity;
        }
    };
}


