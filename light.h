#pragma once
#include "precomp.h"

namespace RaytracingRenderer {
	class Light
	{

	public:

		float intesity;

		Light(float intensity) {
			this->intensity = intensity;
		}

		Light() {}

		~Light() {}

		Light() : Light() {}


	};
};