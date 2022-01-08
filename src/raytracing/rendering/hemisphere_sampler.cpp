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

	inline void createCoordinateSystem(const float3& N, float3& Nt, float3& Nb)
	{
		if (std::fabs(N.x) > std::fabs(N.y))
			Nt = float3(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
		else
			Nt = float3(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
		Nb = cross(N, Nt);
	}
	inline float3 uniformSampleHemisphere(const float& r1, const float& r2)
	{
		// cos(theta) = r1 = y
		// cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
		float sinTheta = sqrtf(1 - r1 * r1);
		float phi = 2 * PI * r2;
		float x = sinTheta * cosf(phi);
		float z = sinTheta * sinf(phi);
		return float3(x, r1, z);
	}
	// TODO: reflect a random position on the hemisphere.
	inline float3 diffuseReflection(float3 normal) {
		float3 Nt, Nb;
		createCoordinateSystem(normal, Nt, Nb);

		float r1 = random_float();
		float r2 = random_float();
		
		float3 random_dir = uniformSampleHemisphere(r1, r2);
		float3 sampleWorld(
			random_dir.x * Nb.x + random_dir.y * normal.x + random_dir.z * Nt.x,
			random_dir.x * Nb.y + random_dir.y * normal.y + random_dir.z * Nt.y,
			random_dir.x * Nb.z + random_dir.y * normal.z + random_dir.z * Nt.z);

		return sampleWorld;
	}

	void HemisphereSampler::Sample(Ray &ray, float3& energy) {

		hit_record rec = hit_record();

		bool collision = scene->intersect(ray, 0.001f, FLT_MAX, rec);
		
		if (collision == false) {
			return;
		}

		float3 diffuseDir = diffuseReflection(rec.normal);

		hit_record diffuse_rec = hit_record();
		Ray diffuse_ray = Ray(rec.p + diffuseDir * 0.0001f, diffuseDir);
		collision = scene->intersect(diffuse_ray, 0.001f, FLT_MAX, diffuse_rec);
		
		energy += (rec.mat_ptr->emission() * rec.mat_ptr->emission_color());

		if (collision == false) {
			return;
		}

		if (diffuse_rec.mat_ptr->emission() > 0)
		{
			float3 pixel_color(0, 0, 0);
			rec.mat_ptr->color(ray, rec, pixel_color);
			float3 BRDF = pixel_color * INVPI;

			float cos_i = dot(diffuseDir, rec.normal);
			float3 reflected_color = (diffuse_rec.mat_ptr->emission() * diffuse_rec.mat_ptr->emission_color());
			energy += 2.f * PI * BRDF * (diffuse_rec.mat_ptr->emission() * diffuse_rec.mat_ptr->emission_color()) * cos_i;
		}
	}



}