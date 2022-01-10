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
				//if (y == 150 && x == 200) {
				//	cout << "Middle sample";
				//}
				energy = Sample(ray, 0);

				float3 pixel_color = accumulator.accumulateEnergy(energy, x, y);
				frame[y][x] = pixel_color;
			}
		}
	}

	float3 PathTracer::Sample(Ray& ray, int bounces) {

		// trace ray
		hit_record rec = hit_record();

		bool collision = scene->intersect(ray, 0.001f, FLT_MAX, rec);

		if (!collision)
			return scene->background_color;

		if (bounces > MAX_RECURSION_DEPTH) {
			return float3(0,0,0);
		}
		
		if (rec.mat_ptr->type() == GLASS) {
			
			float prev_ior, next_ior;
			get_ior(ray, rec, prev_ior, next_ior);
			float reflection, transmission;
			fresnel(ray.dir, rec.normal, prev_ior, next_ior, reflection, transmission);

			float rand_float = random_float();
			bool reflected = rand_float < reflection;

			float3 albedo = rec.mat_ptr->color(ray, rec);
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

			// Absorption (Beer's law)
			if (rec.front_face == false) {
				float3 absorption = rec.mat_ptr->absorption();
				float distance = length(rec.p - ray.orig);
				energy.x *= exp(-absorption.x * distance);
				energy.y *= exp(-absorption.y * distance);
				energy.z *= exp(-absorption.z * distance);
			}

			return energy;
		}

		if (rec.mat_ptr->type() == LIGHT)
			return rec.mat_ptr->emission() * rec.mat_ptr->emission_color();

		if (rec.mat_ptr->type() == SOLID) {

			const float specular = rec.mat_ptr->specularity();
			float rand_float = random_float();
			bool specular_reflect = rand_float < specular;
			bool diffuse_reflect = !specular_reflect;

			float3 albedo = rec.mat_ptr->color(ray, rec);

			if (diffuse_reflect) {
				float diffuse = 1 - specular;

				float3 R = diffuseReflection(rec.normal);
				float3 BRDF = (albedo / PI);
				Ray r(rec.p, R);
				// update throughput
				float3 Ei = Sample(r, bounces + 1) * dot(rec.normal, R);

				return PI * 2.0f * BRDF * Ei;
			}

			if (specular_reflect) {
				Ray r(rec.p, reflect(ray.dir, rec.normal));
				return albedo * Sample(r, bounces + 1);
			}
		}

		return float3(0, 0, 0);
	}
}