#pragma once
#include "precomp.h"

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