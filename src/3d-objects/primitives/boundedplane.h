#pragma once

namespace RaytracingRenderer {

    class BoundedPlane : public Object3D, public Hittable
    {
    public:
        vector<Triangle> tris = vector<Triangle>(2);
        float width;

        inline BoundedPlane(float3 position, float3 v0, float3 v1, float3 v2, float3 v3, shared_ptr<Material> material) : Object3D(position, material) {
            
            this->tris[0] = Triangle(position, v0, v1, v2, material);
            this->tris[1] = Triangle(position, v0, v3, v1, material);
            updateAABB(this->bounding_box);
            this->material = material;
        }

        inline BoundedPlane(float3 position, float width, shared_ptr<Material> material) : Object3D(position, material) {
            this->width = width;
            float3 v0 = float3(-width / 2.f, 0, -width / 2.f);
            float3 v1 = float3(width / 2.f, 0, width / 2.f);
            float3 v2 = float3(-width / 2.f, 0, width / 2.f);
            float3 v3 = float3(width / 2.f, 0, -width / 2.f);
            this->tris[0] = Triangle(position, v0, v2, v1, material);
            this->tris[1] = Triangle(position, v0, v3, v1, material);
            updateAABB(this->bounding_box);
            this->material = material;
        }

        void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const override {
            float d;
            vector<float3> p;
            for (Triangle tri : tris) {
                p.push_back(tri.v0.position);
                p.push_back(tri.v1.position);
                p.push_back(tri.v2.position);
            }
            for (uint32_t i = 0; i < p.size(); ++i) {
                d = dot(planeNormal, p[i]);
                if (d < dnear) dnear = d;
                if (d > dfar) dfar = d;
            }
        }

        bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
            bool check = false;
            for (Triangle tri : tris) {
                if (tri.intersect(ray, t_min, t_max, rec))
                    return true;
            }
            return false;
        }

        void updateAABB(AABB box) const override {
            for (Triangle tri : tris) {
                box.addPoint(tri.v0.position);
                box.addPoint(tri.v1.position);
                box.addPoint(tri.v2.position);
                //tri.updateAABB(box);
            }
            cout << "bounded plane: " << box.p_min << " " << box.p_max << " ";
        }
    };
}
