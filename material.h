#pragma once

namespace RaytracingRenderer {
	
	class Material
	{
	public:
		float3 col;

		Material(float3 col) {
			this->col = col;
		}

		Material(): Material(float3(1, 0, 0)) {}
	};
}