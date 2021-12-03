#pragma once

#include <iostream>

namespace RaytracingRenderer {

    unsigned int translate_color(float3 pixel_color) {
        uint8_t r = pixel_color.x * 255.f;
        uint8_t g = pixel_color.y * 255.f;
        uint8_t b = pixel_color.z * 255.f;

        return (r << 16) + (g << 8) + b;
    }
}

