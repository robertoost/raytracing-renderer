#pragma once
#include "material.h"

namespace RaytracingRenderer {
	class Object3D
	{
	public:
		float3 position;
		Material material;

		Object3D(float3 position, Material material) {
			this->position = position;
			this->material = material;
		}
		Object3D() : position(float3(0,0,0)), material() {}
	};

};