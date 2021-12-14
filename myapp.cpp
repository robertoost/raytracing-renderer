﻿#include "precomp.h"
#include "myapp.h"
#include <future>

TheApp* CreateApp() { return new MyApp(); }

using namespace RaytracingRenderer;

inline float3 refract(const float3& dir, const float3& normal,
	float prev_ior, float next_ior)
{
	const float n = prev_ior / next_ior;
	const float cos_i = dot(normal, dir);
	const float sin_t2 = n * n * (1.0f - cos_i * cos_i);
	if (sin_t2 > 1.0)
	{
		// Refraction doesn't happen. For now, just pass the direction.
		return dir;
	}
	return n * dir - (n + sqrt(1.0f - sin_t2)) * normal;
}

inline void fresnel(const float3& dir, const float3& normal, const float& prev_ior, const float& next_ior, float& reflection, float& transmission)
{
	float cos_i = dot(dir, normal);
	float etai = prev_ior, etat = next_ior;
	if (cos_i > 0) {
		std::swap(etai, etat);
	}

	// Snell's law
	float sin_t = etai / etat * sqrtf(max(0.f, 1.f - cos_i * cos_i));

	// Total internal reflection
	if (sin_t >= 1) {
		reflection = 1;
	}
	
	// Calculate ratio of refraction/reflection
	else {
		float cos_t = sqrtf(max(0.f, 1.f - sin_t * sin_t));
		cos_i = fabsf(cos_i);

		float ni = etat * cos_i;
		float nt = etat * cos_t;
		float pi = etai * cos_i;
		float pt = etai * cos_t;

		float Rs = powf((ni - pt) / (ni + pt), 2.f);
		float Rp = powf((pi - nt) / (pi + nt), 2.f);

		reflection = (Rs * Rs + Rp * Rp) / 2.f;
	}
	transmission = 1 - reflection;
	//reflection *= 10;
}

inline void get_ior(Ray& ray, hit_record& rec, float& prev_ior, float& ior) {
	ior = rec.mat_ptr->ior();
	prev_ior = 1.f;

	// We're inside the material, going out.
	if (ray.mat_ptr.get() != nullptr && ray.mat_ptr->type() == GLASS && rec.front_face == false) {
		prev_ior = ray.mat_ptr->ior();
	}
}

inline float random_float() {
	return rand() / (RAND_MAX + 1.f);
}

float3 MyApp::Trace(Ray &ray) {

	// Trace a ray and record the hit.
	hit_record rec = hit_record();
	const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);
	
	float3 pixel_color = float3(0, 0, 0);

	// If there's no collision, return black.
	if (collision == false) {
		return float3(0.65f, 0.85f, 1.f);
	}

	// If a collision was found, get the color of the object.
	else {

		rec.mat_ptr->color(ray, rec, pixel_color);

		MAT_TYPE mat_type = rec.mat_ptr->type();

		if (mat_type == SOLID) {
			pixel_color *= TraceSolid(ray, rec, 0);
		}
		else if (mat_type == GLASS) {
			pixel_color *= TraceGlass(ray, rec, 0);
		}
	}
	return pixel_color;
}

float3 MyApp::TraceReflection(Ray& ray, uint bounce_count) {

	// Trace the given reflection ray and find the nearest intersection.
	hit_record rec = hit_record();
	const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);
	float3 pixel_color = float3(0,0,0);

	// If there's no collision, return black.
	if (collision == false) {
		return float3(0.65f, 0.85f, 1.f);
	}

	// Initially set the pixel color using the material's color.
	rec.mat_ptr->color(ray, rec, pixel_color);

	if (bounce_count > max_bounces) {
		return pixel_color;
	}

	if (rec.mat_ptr->type() == SOLID) {
		pixel_color *= TraceSolid(ray, rec, bounce_count);
	}
	// Reflect glass.
	if (rec.mat_ptr->type() == GLASS) {
		//cout << "INTERNAL REFLECTION";
		pixel_color *= TraceGlass(ray, rec, bounce_count);
	}

	return pixel_color;
}

float3 MyApp::TraceRefraction(Ray& ray, uint bounce_count) {

	// negative bias to allow for self intersection?
	hit_record rec = hit_record();
	const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);
	float3 pixel_color = float3(0,0,0);

	if (collision == false) {
		return float3(0.65f, 0.85f, 1.f);
	}

	rec.mat_ptr->color(ray, rec, pixel_color);

	if (bounce_count > max_bounces) {
		return pixel_color;
	}

	if (rec.mat_ptr->type() == GLASS) {
		//cout << "GLASS HIT GLASS";
		pixel_color *= TraceGlass(ray, rec, bounce_count);
	}
	else if (rec.mat_ptr->type() == SOLID) {
		pixel_color *= TraceSolid(ray, rec, bounce_count);
	}

	return pixel_color;
}

float3 MyApp::TraceSolid(Ray& ray, hit_record& rec, uint bounce_count) {

	// Get the material's specularity and infer the diffuse value.
	const float specular = rec.mat_ptr->specularity();
	const float diffuse = 1 - specular;

	// Sum both the specular and the diffuse color.
	float3 spec_diff_color = float3(0, 0, 0);

	// Diffuse color value. On the first bounce, add direct illumination.
	if (diffuse > 0 && bounce_count <= max_reflection_shadows) {
		spec_diff_color += diffuse * DirectIllumination(rec.p, rec.normal);
	}
	// If no direct illumination is added, just add the material color.
	else if (diffuse > 0) {
		spec_diff_color += diffuse * float3(1, 1, 1);
	}

	// Specular color value. Trace new ray and increase bounce count to prevent crashing when mirrors face eachother. 
	if (specular > 0) {
		float3 reflect_dir = reflect(ray.dir, rec.normal);
		Ray reflect_ray = Ray(rec.p, reflect_dir, ray.mat_ptr);
		spec_diff_color += specular * TraceReflection(reflect_ray, bounce_count + 1);
	}

	return spec_diff_color;
}

float3 MyApp::TraceGlass(Ray& ray, hit_record& rec, uint bounce_count) {
	float prev_ior, next_ior;
	get_ior(ray, rec, prev_ior, next_ior);
	float reflection, transmission;
	fresnel(ray.dir, rec.normal, prev_ior, next_ior, reflection, transmission);

	float3 glass_pixel_color = float3(0, 0, 0);

	if (transmission > 0) {

		float3 refract_dir = refract(ray.dir, rec.normal, prev_ior, next_ior);
		Ray refract_ray = Ray(rec.p, refract_dir, rec.mat_ptr);

		float3 transmission_color = transmission * TraceRefraction(refract_ray, bounce_count + 1);

		if (rec.front_face == false) {
			float3 absorption = rec.mat_ptr->absorption();
			float distance = length(rec.p - ray.orig);
			transmission_color.x *= exp(-absorption.x * distance);
			transmission_color.y *= exp(-absorption.y * distance);
			transmission_color.z *= exp(-absorption.z * distance);
		}

		glass_pixel_color += transmission_color;
	}

	if (reflection > 0) {
		float3 reflect_dir = reflect(ray.dir, rec.normal);
		Ray reflect_ray = Ray(rec.p, reflect_dir, ray.mat_ptr);
		glass_pixel_color += reflection * TraceReflection(reflect_ray, bounce_count + 1);
	}

	return glass_pixel_color;
}
float3 MyApp::DirectIllumination(float3 &position, float3 &normal) {
	float3 pixel_lighting = float3(0, 0, 0);

	for (shared_ptr<Light> light : scene.lights) {
		pixel_lighting += light->illuminate(position, normal, scene);
	}
	return pixel_lighting;
}


// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void MyApp::Init()
{
	scene = SceneManager::DirectionalLightTest();
	camera = Camera();
}

void MyApp::CalculateColor(Blockjob job)
{
	for (int x = job.rowStart; x < job.rowEnd; ++x)
	{
		for (int y = 0; y < job.colSize; ++y)
		{
			float3 pixel_color = float3(0, 0, 0);
			for (int s = 0; s < job.spp; ++s)
			{
				float offset_x = antialiasing == true ? random_float() : 0.f;
				float offset_y = antialiasing == true ? random_float() : 0.f;

				float u = (x + offset_x) / (((float)SCRWIDTH) - 1.f);
				float v = (y + offset_y) / (((float)SCRHEIGHT) - 1.f);

				float3 screen_point = camera.screen_p0 + u * camera.screen_p1 - camera.screen_p0 + v * camera.screen_p2 - camera.screen_p0;

				// Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
				float3 ray_dir = normalize(screen_point - camera.cameraPos);
				Ray ray = Ray(camera.cameraPos, ray_dir);

				pixel_color += Trace(ray);
			}

			// Final antialiasing division.
			if (antialiasing == true) {
				pixel_color /= samples_per_pixel;
			}
			const unsigned int index = x * job.colSize + y;
			job.indices.push_back(index);
			job.colors.push_back(pixel_color);
		}
	}
	{
		lock_guard<std::mutex> lock(mute);
		imageBlocks.push_back(job);
		completedThreads++;
		cvResults.notify_one();
	}
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void MyApp::Tick( float deltaTime )
{
	// clear the screen to black
	screen->Clear( 0 );

	imageBlocks.clear();
	completedThreads = { 0 };
	threads.clear();


	float3 x_dir = camera.screen_p1 - camera.screen_p0;
	float3 y_dir = camera.screen_p2 - camera.screen_p0;

	float x_width = length(x_dir);
	float y_height = length(y_dir);
	
	if (key_held_down)
	{
		camera.keyHandler(held_key);
	}

	// TEST A SINGLE RAY
	//float3 screen_point = float3(0, 0.1f, 1.f);
	//// Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
	//float3 ray_dir = normalize(screen_point - camera.cameraPos);

	//Ray ray = Ray(camera.cameraPos, ray_dir);

	//Trace(ray)


	// Loop over every pixel in the screen

	vector<float3> image(SCRWIDTH * SCRHEIGHT);

	for (int i = 0; i < nThreads; ++i)
	{	
		Blockjob job;
		job.rowStart = i * rowsPerThread;
		job.rowEnd = job.rowStart + rowsPerThread;

		if (i == nThreads - 1)
		{
			job.rowEnd = job.rowStart + rowsPerThread + leftOver;
		}

		job.colSize = SCRHEIGHT;
		job.spp = antialiasing == true ? samples_per_pixel : 1;

		thread t([this, job]() {
			CalculateColor(job);
			});

		threads.push_back(move(t));
	}

	{
		unique_lock<std::mutex> lock(mute);
		cvResults.wait(lock, [&] {
			return completedThreads == nThreads;
			});
	}

	for (thread &t : threads)
	{
		t.join();
	}

	for (Blockjob job : imageBlocks)
	{
		int index = job.rowStart;
		int colorIndex = 0;
		for (float3 pc : job.colors)
		{
			int colIndex = job.indices[colorIndex];
			image[colIndex] = pc;
			++colorIndex;
		}
	}

	//FIXME: FIX THISSSSSSS!!!! colors don't calculate right???
	for (int y = SCRHEIGHT - 1; y >= 0; --y) {
		int m = 0;
		for (int x = 0; x < SCRWIDTH; ++x) {

			uint c = translate_color(image[m]);

			screen->Plot(x, y, c);
			m++;
		}
	}



	//for (int y = SCRHEIGHT - 1; y >= 0; --y) {
	//	for (int x = 0; x < SCRWIDTH; ++x) {
	//		float3 pixel_color = float3(0, 0, 0);
	//		// 	Point on the screen:
	//		// 𝑃(𝑢, 𝑣) = 𝑃0 + 𝑢(𝑃1 −𝑃0) + 𝑣(𝑃2 −𝑃0)
	//		// 𝑢, 𝑣 ∈[0, 1]

	//		// Loop count with or without antialiasing.
	//		int n = antialiasing == true ? samples_per_pixel : 1;

	//		//ANTI ALIASING
	//		for (int s = 0; s < n; ++s) {
	//			// Antialiasing
	//			float offset_x = antialiasing == true ? random_float() : 0.f;
	//			float offset_y = antialiasing == true ? random_float() : 0.f;

	//			float u = (x + offset_x) / (((float)SCRWIDTH) - 1.f);
	//			float v = (y + offset_y) / (((float)SCRHEIGHT) - 1.f);

	//			float3 screen_point = camera.screen_p0 + u * x_dir + v * y_dir;

	//			// Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
	//			float3 ray_dir = normalize(screen_point - camera.cameraPos);
	//			Ray ray = Ray(camera.cameraPos, ray_dir);

	//			pixel_color += Trace(ray);
	//		}

	//		// Final antialiasing division.
	//		if (antialiasing == true) {
	//			pixel_color /= samples_per_pixel;
	//		}

	//		uint c = translate_color(pixel_color);

	//		screen->Plot(x, y, c);
	//	}
	//}
	 cout << "done";
}

void MyApp::KeyUp(int key)
{
	key_held_down = false;
}
void MyApp::KeyDown(int key)
{
	key_held_down = true;
	held_key = key;
}
void MyApp::MouseMove(int x, int y) 
{
	if (mouse_held_down)
	{
		camera.mouseHandler(x, y);
	}
}
void MyApp::MouseUp(int button)
{
	if (button == 0)
	{
		mouse_held_down = false;
		camera.firstMouse = true;
	}
}
void MyApp::MouseDown(int button)
{
	if (button == 0) 
	{
		mouse_held_down = true;
	}	
}