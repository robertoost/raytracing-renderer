#pragma once
#include "precomp.h"

namespace RaytracingRenderer {
	class Light
	{

	public:

		float3 pos;
		float intensity;
		float ambient_strength;


		Light(float3 pos, float intensity, float ambient_strength) {
			this->pos = pos;
			this->intensity = intensity;
			this->ambient_strength = ambient_strength;
		}

		Light() 
		{
			pos = float3(0., 3., 0.);
			intensity = .6;
			ambient_strength = .2;
		}

		~Light() {}

		Light() : Light() 
		{
			pos = float3(0., 3., 0.);
			intensity = .6;
			ambient_strength = .2;
		}


	};
};