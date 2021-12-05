#pragma once
#pragma once
#include "light.h"

namespace RaytracingRenderer {
	class Spotlight : public Light
	{

	public:

		float3 direction;

		float cutoff;
		float outer_cutoff;

		Spotlight(float3 direction,
			float cutoff, float outer_cutoff) {
			this->direction = direction;
			this->cutoff = cos(cutoff * (PI / 180));
			this->outer_cutoff = cos(outer_cutoff * (PI / 180));
		}

		Spotlight() {}

		~Spotlight() {}

		Spotlight(float3 direction,
			float cutoff, float outer_cutoff) : Light()
		{
			direction = direction;
			cutoff = cos(cutoff * (PI / 180));
			outer_cutoff = cos(outer_cutoff * (PI / 180));
		}
	};
};