#pragma once

#include <iostream>

namespace RaytracingRenderer {

    unsigned int translate_color(float3 pixel_color) {
        const uint ir = min((uint)(pixel_color.x * 255), 255u);
        const uint ig = min((uint)(pixel_color.y * 255), 255u);
        const uint ib = min((uint)(pixel_color.z * 255), 255u);
        return (ir << 16) + (ig << 8) + ib;
    }
}

