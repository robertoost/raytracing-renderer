#pragma once
#include "precomp.h"
#include "object3d.h"

namespace RaytracingRenderer {
	class Light
	{

	public:

		Light(const float3 &p, const float3 &i) : position(p), intensity(i) {}

		float3 position;
		float3 intensity;
		
		
		//float ambient_strength;
		//Light(float3 pos, float3 intensity) {
		//	this->pos = pos;
		//	this->intensity = intensity;
		//	//this->ambient_strength = ambient_strength;
		//}

		Light() {}

		//~Light() {}

		//Light() : pos(0., 3., 0.), intensity(.6, .6, .6) {}

	};


	/**
	 * Return vector of reflection direction of an object's surface.
	 *
	 * @param values direction of ray and object's normal.
	 * @return direction of reflection.
	 */
	float3 reflectVector(const float3& I, const float3& N)
	{
		return I - 2 * dot(I, N) * N;
	}

	/**
	 * Return refraction direction.
	 *
	 * @param values direction of ray, object's normal, material ior value.
	 * @return direction of reflection.
	 */
	float3 refract(const float3& I, const float3& N, const float& ior)
	{
		float cosi = clamp(-1., 1., dot(I, N));
		float etai = 1, etat = ior;
		float3 n = N;
		if (cosi < 0) 
		{ 
			cosi = -cosi; 
		}
		else { 
			swap(etai, etat); 
			n = float3(0,0,0) - N; 
		}
		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);
		return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
	}

	/**
	* Compute Fresnel equation.
	*
	* @param values I (incident view direction), N (normal at the intersection point), ior (material refractive index), Kr (amount of light reflected)
	* @return void
	*/
	void fresnel(const float3 & I, const float3 & N, const float& ior, float& kr)
	{
		float cosi = clamp(-1., 1., dot(I, N));
		float etai = 1, etat = ior;
		if (cosi > 0) { std::swap(etai, etat); }
		// Compute sini using Snell's law
		float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
		// Total internal reflection
		if (sint >= 1) {
			kr = 1;
		}
		else {
			float cost = sqrtf(std::max(0.f, 1 - sint * sint));
			cosi = fabsf(cosi);
			float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
			float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
			kr = (Rs * Rs + Rp * Rp) / 2;
		}
		// As a consequence of the conservation of energy, transmittance is given by:
		// kt = 1 - kr;
	}

	bool trace(
		const Ray& ray,
		const vector<unique_ptr<Object3D>>& objects,
		float& tNear, uint32_t& index, float2 & uv, Object3D** hitObject)
	{
		*hitObject = nullptr;
		for (uint32_t k = 0; k < objects.size(); ++k) {
			float tNearK = INFINITY;
			uint32_t indexK;
			float2 uvK;
			if (objects[k]->intersect(ray, tNearK, indexK, uvK) && tNearK < tNear) {
				*hitObject = objects[k].get();
				tNear = tNearK;
				index = indexK;
				uv = uvK;
			}
		}

		return (*hitObject != nullptr);
	}

	/**
	* Whitted-style light transport algorithm
	*
	* @param values orig (ray origin), dir (ray direction), objects (vector of objects in scene), lights (vector of lights in scene), depth, test ( = false)
	* @return color at the intersection point
	*/
	float3 castRay(const Ray& ray,
		const std::vector<std::unique_ptr<Object3D>>& objects,
		const std::vector<std::unique_ptr<Light>>& lights,
		const Options& options,
		uint32_t depth,
		bool test = false)
	{
		if (depth > options.maxDepth) {
			return options.backgroundColor;
		}

		float3 hitColor = options.backgroundColor;
		float tnear = INFINITY;
		float2 uv;
		uint32_t index = 0;
		Object3D* hitObject = nullptr;
		if (trace(ray, objects, tnear, index, uv, &hitObject)) {
			float3 hitPoint = ray.orig + ray.dir * tnear;
			float3 N; // normal 
			float3 st; // st coordinates 
			hitObject->getSurfaceProperties(hitPoint, ray.orig, index, uv, N, st);
			float3 tmp = hitPoint;
			switch (hitObject->material->materialType) {
			case REFLECTION_AND_REFRACTION:
			{
				float3 reflectionDirection = normalize(reflectVector(ray.orig, N));
				float3 refractionDirection = normalize(refract(ray.orig, N, hitObject->material->ior));
				float3 reflectionRayOrig = (dot(reflectionDirection, N) < 0) ?
					hitPoint - N * options.bias :
					hitPoint + N * options.bias;
				float3 refractionRayOrig = (dot(refractionDirection, N) < 0) ?
					hitPoint - N * options.bias :
					hitPoint + N * options.bias;
				float3 reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1, 1);
				float3 refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1, 1);
				float kr;
				fresnel(ray.orig, N, hitObject->material->ior, kr);
				hitColor = reflectionColor * kr + refractionColor * (1 - kr);
				break;
			}
			case REFLECTION:
			{
				float kr;
				fresnel(dir, N, hitObject->material->ior, kr);
				float3 reflectionDirection = reflectVector(dir, N);
				float3 reflectionRayOrig = (dot(reflectionDirection, N) < 0) ?
					hitPoint + N * options.bias :
					hitPoint - N * options.bias;
				hitColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1) * kr;
				break;
			}
			default:
			{
				float3 lightAmt = float3(0, 0, 0);
				float3 specularColor = float3(0, 0, 0);
				float3 shadowPointOrig = (dot(dir, N) < 0) ?
					hitPoint + N * options.bias :
					hitPoint - N * options.bias;
				for (uint32_t i = 0; i < lights.size(); ++i) {
					float3 lightDir = lights[i]->position - hitPoint;
					// square of the distance between hitPoint and the light
					float lightDistance2 = dot(lightDir, lightDir);
					lightDir = normalize(lightDir);
					float LdotN = max(0.f, dot(lightDir, N));
					Object3D* shadowHitObject = nullptr;
					float tNearShadow = INFINITY;
					// is the point in shadow, and is the nearest occluding object closer to the object than the light itself?
					bool inShadow = trace(shadowPointOrig, lightDir, objects, tNearShadow, index, uv, &shadowHitObject) &&
						tNearShadow * tNearShadow < lightDistance2;
					lightAmt += (1 - inShadow) * lights[i]->intensity * LdotN;
					float3 reflectionDirection = reflect(-lightDir, N);
					specularColor += powf(max(0.f, -dot(reflectionDirection, dir)), hitObject->material->specularExponent) * lights[i]->intensity;
				}
				hitColor = lightAmt * hitObject->material->diffuseColor * hitObject->material->Kd + specularColor * hitObject->material->Ks;
				break;
			}
			}
		}

		return hitColor;
	}
	}
};