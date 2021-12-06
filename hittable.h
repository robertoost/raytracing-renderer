#pragma once

namespace RaytracingRenderer {
    class Material;

    struct hit_record {
        float3 p;
        float3 normal;
        double t;
        shared_ptr<Material> mat_ptr;
    };

    class Hittable {
    public:
        virtual bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
    };
}