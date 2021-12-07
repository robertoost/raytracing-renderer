#pragma once

namespace RaytracingRenderer {

	class Light
	{

	public:

		float3 position;
		float intensity;

		Light(const float3 &position, const float intensity) {
			this->position = position;
			this->intensity = intensity;
		}
		
		Light() : Light(float3(0, 0, 0), 1.f) {}
		
		virtual float3 illuminate(float3& position, float3& normal, Scene& scene) = 0;
	};
}