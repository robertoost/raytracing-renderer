#pragma once

namespace RaytracingRenderer {

    inline uint translate_color(float3 pixel_color) {
        const uchar ir = min((uint)(pixel_color.x * 255), 255u);
        const uchar ig = min((uint)(pixel_color.y * 255), 255u);
        const uchar ib = min((uint)(pixel_color.z * 255), 255u);
        return (ir << 16) + (ig << 8) + ib;
    }

    inline float total_energy(float3 frame[SCRHEIGHT][SCRWIDTH]) {
        float total =0;

        for (int y = 0; y < SCRHEIGHT; y++) {
            for (int x = 0; x < SCRWIDTH; x++) {
                float3 color = frame[y][x];
                total += color.x + color.y + color.z;
            }
        }

        return total;
    }

}
