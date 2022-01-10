#pragma once

namespace RaytracingRenderer {

	class PathTracer: public Renderer
	{
	public:
		inline PathTracer(Scene& scene, Camera& camera) : Renderer(scene, camera), accumulator() {}
		inline PathTracer() : Renderer(), accumulator() {}
		// When the camera updates, the accumulated energy is reset.
		inline void OnCameraUpdate() override { accumulator.resetAccumulator(); };
		void RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) override;
		float3 Sample(Ray& ray, int bounces);
	private:
		Accumulator accumulator;
	};
}

