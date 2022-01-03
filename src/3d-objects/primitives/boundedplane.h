#pragma once

namespace RaytracingRenderer {

    class BoundedPlane : public Object3D, public Hittable
    {
    public:
        vector<Triangle> tris = vector<Triangle>(2);

        inline BoundedPlane(float3 position, float3 v0, float3 v1, float3 v2, float3 v3, shared_ptr<Material> material) : Object3D(position, material) {
            
            this->tris[0] = Triangle(position, v0, v1, v2, material);
            this->tris[1] = Triangle(position, v0, v3, v1, material);
        }

        bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
            bool check = false;
            for (Triangle tri : tris) {
                check = tri.intersect(ray, t_min, t_max, rec);
                if (check)
                    return true;
            }
            return check;
        }

    };
}
