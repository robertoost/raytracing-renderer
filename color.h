#pragma once

namespace RaytracingRenderer {

    inline uint translate_color(float3 pixel_color) {
        const uchar ir = min((uint)(pixel_color.x * 255), 255u);
        const uchar ig = min((uint)(pixel_color.y * 255), 255u);
        const uchar ib = min((uint)(pixel_color.z * 255), 255u);
        return (ir << 16) + (ig << 8) + ib;
    }
}
