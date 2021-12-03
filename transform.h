#pragma once

namespace RaytracingRenderer {

	class Transform
	{
	public: 
		float3 position;
		// TODO: ROTATION, SCALE
		Transform(float3 position) {
			this->position = position;
		}
		Transform() {
			this->position = float3(0., 0., 0.);
		}
	};
}

