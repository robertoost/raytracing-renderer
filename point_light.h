#pragma once

namespace RaytracingRenderer {

    class PointLight : public Light
    {
    public:
        PointLight(float3 position, float intensity) : Light(position, intensity) {}

        PointLight() : Light() {}

        float3 illuminate(float3& position, float3& normal, Scene& scene) {

            // Get difference between light and position.
            float3 diff = this->position - position;
            float distance_to_light = length(diff);
            float3 direction = normalize(diff);

            // Prepare shadow raycast
            Ray shadow_ray = Ray(position, direction);
            hit_record rec = hit_record();

            // Get collision.
            bool collision = scene.intersect(shadow_ray, 0.0001, distance_to_light, rec);

            // If the light is obscured, continue.
            if (collision == true) {
                return float3(0,0,0);
            }


            float cos_a = clamp(dot(normal, direction), 0.f, 1.f);

            if (cos_a == 0) {
                return float3(0, 0, 0);
            }
            return float3(1, 1, 1) * cos_a * intensity / powf(distance_to_light, 2.f);
        }
    };
}


