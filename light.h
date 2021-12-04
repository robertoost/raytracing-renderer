#pragma once
#include "precomp.h"

namespace RaytracingRenderer {
	class Light
	{

	public:

		float3 direction;

		Light(float3 direction) {
			this->direction = direction;
		}

		Light() {}

		~Light() {}

		Light() : Light() {}


	};
};