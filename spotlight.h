#pragma once
#pragma once
#include "light.h"
#include "camera.h"

namespace RaytracingRenderer {
	class Spotlight : public Light
	{

	public:

		float3 pos;
		float3 direction;

		float cutoff;
		float outer_cutoff;

		Spotlight(float3 pos,
			float3 direction,
			float cutoff, float outer_cutoff) {
			this->pos = pos;
			this->direction = direction;
			this->cutoff = cutoff;
			this->outer_cutoff = outer_cutoff;
		}

		Spotlight() {}

		~Spotlight() {}

		Spotlight() : Light() 
		{
			pos = float3(0.f, 0.f, 0.f); 
			direction = float3(0.f, 0.f, 1.f); 
			cutoff = cos(12.5f * (PI / 180)); 
			outer_cutoff = cos(17.5f * (PI / 180));
		}


	};
};