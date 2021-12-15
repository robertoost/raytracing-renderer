#include "precomp.h"

namespace RaytracingRenderer {
	void PostProcessing::gammaCorrection(float3 frame[SCRHEIGHT][SCRWIDTH], float gamma) {
		float power = 1.f / gamma;
		for (uint y = 0; y < SCRHEIGHT; y++) {
			for (uint x = 0; x < SCRWIDTH; x++) {

				// Gamma correction.
				float3 color = frame[y][x];
				color.x = powf(color.x, power);
				color.y = powf(color.y, power);
				color.z = powf(color.z, power);
				frame[y][x] = color;
			}
		}
	}

	void PostProcessing::vignetting(float3 frame[SCRHEIGHT][SCRWIDTH], float2& center) {
		float min_vignette = 1 - MAX_VIGNETTE;
		float max_distance = length(center);

		for (uint y = 0; y < SCRHEIGHT; y++) {
			for (uint x = 0; x < SCRWIDTH; x++) {

				// Make sure the vignette is darkest at the furthest distance from the center.
				float distance = fabsf(length(center - float2(y, x)));
				float vignette = 1 - clamp((distance / max_distance), 0.f, 1.f);
				vignette = lerp(min_vignette, 1.f, vignette);
				frame[y][x] *= vignette;
			}
		}
	}

	void PostProcessing::chromaticAberration(float3 frame[SCRHEIGHT][SCRWIDTH], int2& r_offset, int2& g_offset, int2& b_offset) {
		float max_distance = fmaxf(SCRHEIGHT, SCRWIDTH) / 2;
		float2 center = float2(SCRHEIGHT / 2.f, SCRWIDTH / 2.f);

		// Make sure we don't try to take pixel indices outside the frame.
		int y_min = min(min(r_offset.y, g_offset.y), b_offset.y);
		int y_max = max(max(r_offset.y, g_offset.y), b_offset.y);
		int x_min = min(min(r_offset.x, g_offset.x), b_offset.x);
		int x_max = max(max(r_offset.x, g_offset.x), b_offset.x);

		for (uint y = y_max; y < SCRHEIGHT - y_min; y++) {
			for (uint x = x_max; x < SCRWIDTH - x_min; x++) {
				float distance = fabsf(length(center - float2(y, x)));

				// Get rgb values with offset.
				float r = frame[y + r_offset.y][x + r_offset.x].x;
				float g = frame[y + g_offset.y][x + g_offset.x].y;
				float b = frame[y + b_offset.y][x + b_offset.x].z;

				// Aberration is worse near the edges of the screen.
				float aberration = 1 - clamp((distance / max_distance), 0.f, 1.f);
				float3 color = frame[y][x] * aberration;
				frame[y][x] = color + (1 - aberration) * float3(r,g,b);
			}
		}
	}
}