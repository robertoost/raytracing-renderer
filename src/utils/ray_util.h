#pragma once

namespace RaytracingRenderer {

    inline float3 refract(const float3& dir, const float3& normal,
        float prev_ior, float next_ior)
    {
        const float n = prev_ior / next_ior;
        const float cos_i = dot(normal, dir);
        const float sin_t2 = n * n * (1.0f - cos_i * cos_i);
        if (sin_t2 > 1.0)
        {
            // Refraction doesn't happen. For now, just pass the direction.
            return dir;
        }

        float3 refract_dir = normalize(n * dir - (n + sqrt(1.0f - sin_t2)) * normal);

        return refract_dir;
    }

    inline void fresnel(const float3& dir, const float3& normal, const float& prev_ior, const float& next_ior, float& reflection, float& transmission)
    {
        float cos_i = dot(dir, normal);
        float etai = prev_ior, etat = next_ior;
        if (cos_i > 0) {
            swap(etai, etat);
        }

        // Snell's law
        float sin_t = etai / etat * sqrtf(max(0.f, 1.f - cos_i * cos_i));

        // Total internal reflection
        if (sin_t >= 1) {
            reflection = 1;
        }

        // Calculate ratio of refraction/reflection
        else {
            float cos_t = sqrtf(max(0.f, 1.f - sin_t * sin_t));
            cos_i = fabsf(cos_i);

            float ni = etat * cos_i;
            float nt = etat * cos_t;
            float pi = etai * cos_i;
            float pt = etai * cos_t;

            float Rs = powf((ni - pt) / (ni + pt), 2.f);
            float Rp = powf((pi - nt) / (pi + nt), 2.f);

            reflection = (Rs * Rs + Rp * Rp) / 2.f;
        }
        transmission = 1 - reflection;
        //reflection *= 10;
    }

    inline void get_ior(Ray& ray, hit_record& rec, float& prev_ior, float& ior) {
        ior = rec.mat_ptr->ior();
        prev_ior = 1.f;

        // We're inside the material, going out.
        if (rec.front_face == false) {
            prev_ior = rec.mat_ptr->ior();
            ior = 1.f;
        }
    }

    inline float random_float() {
        return rand() / (RAND_MAX + 1.f);
    }

    //inline float random_float(float min, float max) {
    //    return lerp(min, max, random_float());
    //}
}
