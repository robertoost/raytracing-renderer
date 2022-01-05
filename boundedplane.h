#pragma once

namespace RaytracingRenderer {

    class BoundedPlane : public Triangle
    {
    public:
        float3 normal;

        inline BoundedPlane() : Object3D(), normal(float3(0, 1, 0)) {}
        inline BoundedPlane(float3 position, float3 normal, shared_ptr<Material> material) : Object3D(position, material), normal(normal) {}


        

    };
}

#pragma once
