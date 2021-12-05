#pragma once

namespace RaytracingRenderer {

	enum MaterialType { DIFFUSE_AND_GLOSSY, REFLECTION_AND_REFRACTION, REFLECTION };
	
	class Material
	{
	public:
		MaterialType materialType;
		float ior;
		float Kd, Ks; 
		float3 diffuseColor;
		float specularExponent;

		Material() : materialType(DIFFUSE_AND_GLOSSY), ior(1.3), Kd(.8), Ks(.2), diffuseColor(.2, .2, .2), specularExponent(25) {}

	};
}