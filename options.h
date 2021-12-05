#pragma once

namespace RaytracingRenderer {

    struct Options
    {
        float width;
        float height;
        float fov;
        float imageAspectRatio;
        float maxDepth;
        float3 backgroundColor;
        float bias;
    };
}