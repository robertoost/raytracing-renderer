#pragma once
#pragma once
#include "light.h"

namespace RaytracingRenderer {
	class DirectionalLight : public Light
	{

	public:

		float3 direction;

		float cutoff;
		float outer_cutoff;

		DirectionalLight(float3 direction) {
			this->direction = direction;
		}

		DirectionalLight() {}

		~DirectionalLight() {}

		DirectionalLight() : Light()
		{
			direction = float3(0.f, 0.f, 1.f);
		}


	};
};