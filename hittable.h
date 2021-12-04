#pragma once
#include "ray.h"

namespace RaytracingRenderer {
    struct hit_record {
        float3 p;
        float3 normal;
        double t;
    };

    class Hittable {
    public:
        virtual bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
    };
}