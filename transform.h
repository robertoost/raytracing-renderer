#pragma once
#include <vec3.h>

namespace RaytracingRenderer {

	class Transform
	{
	public: 
		point3 position;
		// TODO: ROTATION, SCALE
		Transform(point3 position) {
			this->position = position;
		}
		Transform() {
			this->position = point3(0., 0., 0.);
		}
	};
}

