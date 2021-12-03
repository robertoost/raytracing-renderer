#pragma once
#include "transform.h"
#include "material.h"
#include <ray.h>

namespace RaytracingRenderer {
	class Object3D
	{
	public:
		Transform transform;
		Material material;

		Object3D(float3 position) : transform(position) {}
		Object3D() : transform() {}

		virtual bool intersect(Ray ray) = 0;
	};

};