#pragma once
#include "material.h"

namespace RaytracingRenderer {
	class Object3D
	{
	public:
		float3 position;
		shared_ptr<Material> material;

		Object3D(float3 position, Material material) {
			this->position = position;
			this->material = make_shared<Material>(material);
		}
		Object3D() : position(float3(0,0,0)), material() {}

	};

};