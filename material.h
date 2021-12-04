#pragma once
#include "camera.h"

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

		Material(float3 col) {}

		~Material() {}

		Material(): Material(float3(1.f, 0.5f, 0.3f), float3(1.f, 0.5f, 0.3f), float3(0.5f, 0.5f, 0.5f), 32.0f, float3(1, 0, 0)) {}

		void getFragColor(Camera camera)
		{
			float3 ambient = lightColor + this->ambient;

			float3 norm = normalize(Normal);
			float3 lightDir = normalize(lightPos - FragPos);
			float diff = max(dot(norm, lightDir), 0.f);
			float3 diffuse = lightcolor * (diff * this->diffuse);

			float3 viewDir = normalize(camera.origin - FragPos);
			float3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.f), this->shininess);
			float3 specular = lightColor * (spec * this->specular);

			float3 result = ambient + diffuse + specular;
			FragColor = make_float4(result, 1.f);
		}
	};
}