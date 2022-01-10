#include "precomp.h"

namespace RaytracingRenderer {
	void PathTracer::RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) {

		accumulator.incrementAccumulation();
		float3 x_dir = camera->screen_p1 - camera->screen_p0;
		float3 y_dir = camera->screen_p2 - camera->screen_p0;

		for (int y = SCRHEIGHT - 1; y >= 0; --y) {
			for (int x = 0; x < SCRWIDTH; ++x) {

				float3 energy = float3(0, 0, 0);

				float u = x / (((float)SCRWIDTH) - 1.f);
				float v = y / (((float)SCRHEIGHT) - 1.f);

				float3 screen_point = camera->screen_p0 + u * x_dir + v * y_dir;

				// Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
				float3 ray_dir = normalize(screen_point - camera->cameraPos);
				Ray ray = Ray(camera->cameraPos, ray_dir);

				energy = Sample(ray, 0);

				// Add the energy to the accumulated average energy, then get the new average value.
				float3 pixel_color = accumulator.accumulateEnergy(energy, x, y);
				frame[y][x] = pixel_color;
			}
		}
	}

	float3 PathTracer::Sample(Ray& ray, int bounces) {

		// Trace ray for a new sample.
		hit_record rec = hit_record();
		bool collision = scene->intersect(ray, 0.001f, FLT_MAX, rec);

		// End recursion if no collision was found.
		if (!collision)
			return scene->background_color;

		// We found a light source, end recursion and return light emission value.
		if (rec.mat_ptr->type() == LIGHT)
			return rec.mat_ptr->emission() * rec.mat_ptr->emission_color();

		// Reached max recursion without finding the background or a light source, so we return black.
		if (bounces > MAX_RECURSION_DEPTH)
			return float3(0,0,0);

		// We hit a dielectric material.
		if (rec.mat_ptr->type() == GLASS) {
			
			// Get ior values and calculate reflection and transmission (refraction).
			float prev_ior, next_ior;
			get_ior(ray, rec, prev_ior, next_ior);
			float reflection, transmission;
			fresnel(ray.dir, rec.normal, prev_ior, next_ior, reflection, transmission);

			// Randomly determine whether we'll reflect or refract, using the fresnel values as probabilities.
			float rand_float = random_float();
			bool reflected = rand_float < reflection;

			float3 energy = float3(0, 0, 0);

			if (reflected) {
				// Reflection
				float3 reflect_dir = reflect(ray.dir, rec.normal);
				Ray reflect_ray = Ray(rec.p, reflect_dir);
				energy = Sample(reflect_ray, bounces + 1);
			}
			else {
				// Refraction.
				float3 refract_dir = refract(ray.dir, rec.normal, prev_ior, next_ior);
				Ray refract_ray = Ray(rec.p, refract_dir);
				energy = Sample(refract_ray, bounces + 1);
			}

			// Absorption (Beer's law). If we're inside the glass, absorb energy over distance.
			if (rec.front_face == false) {
				float3 absorption = rec.mat_ptr->absorption();
				float distance = length(rec.p - ray.orig);
				energy.x *= exp(-absorption.x * distance);
				energy.y *= exp(-absorption.y * distance);
				energy.z *= exp(-absorption.z * distance);
			}

			return energy;
		}

		// We hit a specular/diffuse material.
		if (rec.mat_ptr->type() == SOLID) {

			// Randomly determine whether we'll choose specular or diffuse reflection based on the material's specular value.
			const float specular = rec.mat_ptr->specularity();
			float rand_float = random_float();
			bool specular_reflect = rand_float < specular;

			float3 albedo = rec.mat_ptr->color(ray, rec);

			// Specular samples are reflected using the incident direction and the normal.
			if (specular_reflect) {
				Ray specular_ray(rec.p, reflect(ray.dir, rec.normal));
				return albedo * Sample(specular_ray, bounces + 1);
			} 
			// Diffuse samples are reflected randomly across the hemisphere.
			else {
				float diffuse = 1 - specular;

				float3 diffuse_dir = diffuseReflection(rec.normal);
				float3 BRDF = (albedo / PI);
				Ray diffuse_ray(rec.p, diffuse_dir);

				// Update throughput
				float3 Ei = Sample(diffuse_ray, bounces + 1) * dot(rec.normal, diffuse_dir);

				return PI * 2.0f * BRDF * Ei;
			}
		}

		return float3(0, 0, 0);
	}
}