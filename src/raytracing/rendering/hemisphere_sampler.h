#pragma once

namespace RaytracingRenderer {
	class HemisphereSampler : public Renderer {
	public:
		inline HemisphereSampler(Scene& scene, Camera& camera) : Renderer(scene, camera), accumulator() {}
		inline HemisphereSampler() : Renderer(), accumulator() {}
		// When the camera updates, the accumulated energy is reset.
		inline void OnCameraUpdate() override { accumulator.resetAccumulator(); };
		void RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) override;
		void Sample(Ray& ray, float3& pixel_color);
	private:
		Accumulator accumulator;
	};
}