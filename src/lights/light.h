#pragma once

namespace RaytracingRenderer {

	class Light
	{

	public:

		float3 position;
		float intensity;

		inline Light(const float3 &position, const float intensity) {
			this->position = position;
			this->intensity = intensity;
		}
		
		inline Light() : Light(float3(0, 0, 0), 1.f) {}

		inline Light(const float intensity) {
			this->position = float3(0, 0, 0);
			this->intensity = intensity;
		}
		
		inline virtual float3 illuminate(float3& position, float3& normal, Scene& scene) = 0;
	};
}