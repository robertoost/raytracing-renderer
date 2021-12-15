#pragma once

namespace RaytracingRenderer {

    class DirectionalLight : public Light
    {
    private:
        float3 direction;
    public:
        inline DirectionalLight(const float3 direction, const float intensity) : Light(intensity) {
            this->direction = normalize(direction);
        }

        inline DirectionalLight() : DirectionalLight(float3(-0.2f,-0.7f,-0.1f), 1.f) {}

        inline float3 illuminate(float3& position, float3& normal, Scene& scene) {
            // Prepare shadow raycast
            Ray shadow_ray = Ray(position, -direction);
            hit_record rec = hit_record();

            // Get collision.
            bool collision = scene.intersect(shadow_ray, 0.0001, FLT_MAX, rec);

            // If the light is obscured, continue.
            if (collision == true) {
                return float3(0, 0, 0);
            }

            float cos_a = clamp(dot(normal, -direction), 0.f, 1.f);

            if (cos_a == 0) {
                return float3(0, 0, 0);
            }
            return float3(1, 1, 1) * cos_a * intensity;
        }
    };
}


