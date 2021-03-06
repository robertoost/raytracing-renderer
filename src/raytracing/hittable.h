#pragma once

namespace RaytracingRenderer {
    // Forward instancing.
    class Material;
    class AABB;

    struct hit_record {
        float3 p = float3(0.f,0.f,0.f);
        float3 normal = float3(0.f,0.f,0.f);
        float t = FLT_MAX;
        bool front_face = true;
        shared_ptr<Material> mat_ptr;

        inline void set_face_normal(const Ray& ray, const float3& outward_normal) {
            front_face = dot(ray.dir, outward_normal) < 0;
            normal = front_face ? outward_normal : -1 * outward_normal;
        }
    };

    class Hittable {
    public:
        virtual bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const = 0;

        AABB bounding_box;

        virtual void updateAABB(AABB box) const = 0;

        virtual void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const = 0;

    };
}