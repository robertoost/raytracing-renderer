#pragma once

namespace RaytracingRenderer {
    // Forward instancing.
    class Material;

    struct hit_record {
        float3 p;
        float3 normal;
        double t;
        bool front_face;
        shared_ptr<Material> mat_ptr;

        inline void set_face_normal(const Ray& ray, const float3& outward_normal) {
            front_face = dot(ray.dir, outward_normal) < 0;
            normal = front_face ? outward_normal : -1 * outward_normal;
        }
    };

    class Hittable {
    public:
        virtual bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;
    };
}