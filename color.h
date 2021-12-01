#pragma once

#include "vec3.h"
#include <iostream>

namespace RaytracingRenderer {

    unsigned int translate_color(color pixel_color) {
        uint8_t r = pixel_color.x() * 255.f;
        uint8_t g = pixel_color.y() * 255.f;
        uint8_t b = pixel_color.z() * 255.f;

        return (r << 16) + (g << 8) + b;
    }
}

