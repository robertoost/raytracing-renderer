#pragma once

namespace RaytracingRenderer {
	class Object3D
	{
	public:
		float3 position;
		shared_ptr<Material> material;

		inline Object3D(float3 position, shared_ptr<Material> material) {
			this->position = position;
			this->material = material;
		}
		inline Object3D() : Object3D(float3(), make_shared<DiffuseMaterial>()) {}
	};

};