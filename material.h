#pragma once

namespace RaytracingRenderer {
	
	class Material
	{
	public:
		float3 ambient;
		float3 diffuse;
		float3 specular;
		float shininess;
		float3 col;

		Material(float3 ambient,
		float3 diffuse,
		float3 specular,
		float shininess,
		float3 col) {
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->shininess = shininess;
			this->col = col;
		}

		Material(float3 col) {
			this->col = col;
		}

		~Material() {}

		Material(): Material(float3(1.f, 0.5f, 0.3f), float3(1.f, 0.5f, 0.3f), float3(0.5f, 0.5f, 0.5f), 32.0f, float3(1, 0, 0)) {}

	};
}