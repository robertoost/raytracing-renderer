#pragma once

namespace RaytracingRenderer {
	class PostProcessing
	{
	public:
		static void gammaCorrection(float3 frame[SCRHEIGHT][SCRWIDTH], float gamma);
		static void	chromaticAberration(float3 frame[SCRHEIGHT][SCRWIDTH], int2& r_offset, int2& g_offset, int2& b_offset);
		static void vignetting(float3 frame[SCRHEIGHT][SCRWIDTH], float2& center);
	};
}