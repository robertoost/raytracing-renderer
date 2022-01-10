#include "precomp.h"

namespace RaytracingRenderer {
	void HemisphereSampler::RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) {
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

				Sample(ray, energy);

				float3 pixel_color = accumulator.accumulateEnergy(energy, x, y);
				frame[y][x] = pixel_color;
			}
		}
	}

	void HemisphereSampler::Sample(Ray &ray, float3& energy) {

		hit_record rec = hit_record();

		bool collision = scene->intersect(ray, 0.001f, FLT_MAX, rec);
		
		if (collision == false) {
			return;
		}

		if (rec.mat_ptr->emission() > 0) {
			energy += (rec.mat_ptr->emission() * rec.mat_ptr->emission_color());
			return;
		}

		float3 diffuseDir = diffuseReflection(rec.normal);

		hit_record diffuse_rec = hit_record();
		Ray diffuse_ray = Ray(rec.p, diffuseDir);
		collision = scene->intersect(diffuse_ray, 0.001f, FLT_MAX, diffuse_rec);

		if (collision == false) {
			return;
		}

		if (diffuse_rec.mat_ptr->emission() > 0)
		{
			float3 pixel_color = rec.mat_ptr->color(ray, rec);
			float3 BRDF = pixel_color * INVPI;
			float cos_i = dot(diffuseDir, rec.normal);
			float3 reflected_color = (diffuse_rec.mat_ptr->emission() * diffuse_rec.mat_ptr->emission_color());
			energy += 2.f * PI * BRDF * (diffuse_rec.mat_ptr->emission() * diffuse_rec.mat_ptr->emission_color()) * cos_i;
		}
	}
}