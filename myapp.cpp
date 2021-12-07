﻿#include "precomp.h"
#include "myapp.h"

TheApp* CreateApp() { return new MyApp(); }

using namespace RaytracingRenderer;

float3 MyApp::BackgroundColor(Ray& ray, float3& pixel_color) {
	// Create a nice background color.
	auto t = 0.5f * (ray.dir.y + 1);
	pixel_color = (1 - t) * float3(1, 1, 1) + t * float3(0.5f, 0.7f, 1);
	return pixel_color;
}

float3 MyApp::Trace(Ray &ray) {

	// Trace a ray and record the hit.
	hit_record rec = hit_record();
	const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);

	float3 pixel_color = float3(0, 0, 0);

	// If no collision was found for this ray, draw a nice BG color.
	if (collision == false) {
		return BackgroundColor(ray, pixel_color);
	}
	// If a collision was found, get the color of the object.
	else {
		rec.mat_ptr->color(ray, rec, pixel_color);

		MAT_TYPE mat_type = rec.mat_ptr->type();

		if (rec.mat_ptr->type() == DIFFUSE) {
			// Diffuse materials don't need extra rays.
			//pixel_color *= DirectIllumination(rec.p, rec.normal);
		}
		else if (rec.mat_ptr->type() == MIRROR) {
			// Mirror material, scatters rays.
			float3 reflect_dir = reflect(rec.p, rec.normal);
			Ray reflect_ray = Ray(ray.dir, reflect_dir);
			pixel_color *= Trace(reflect_ray, 1);
		}
		else if (rec.mat_ptr->type() == GLASS) {
			// TODO: GLASS?????
		}
	}
	return pixel_color;
}

float3 MyApp::Trace(Ray& ray, int bounce_count) {

	hit_record rec = hit_record();
	const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);
	float3 pixel_color = float3(0,0,0);

	if (collision == false) {
		return pixel_color;
	}

	rec.mat_ptr->color(ray, rec, pixel_color);

	if (bounce_count <= max_bounces && rec.mat_ptr->type() == MIRROR) {
		float3 reflect_dir = reflect(rec.p, rec.normal);
		Ray reflect_ray = Ray(ray.dir, reflect_dir);
		pixel_color *= Trace(reflect_ray, bounce_count + 1);
	}

	return pixel_color;
}

float3 MyApp::DirectIllumination(float3 &position, float3 &normal) {
	float3 pixel_lighting = float3(0, 0, 0);
	for (shared_ptr<Light> light : scene.lights) {

		// Get difference between light and position.
		float3 diff = light->position - position;
		float distance_to_light = length(diff);
		float3 direction = normalize(diff);

		// Prepare shadow raycast
		Ray shadow_ray = Ray(position, direction);
		hit_record rec = hit_record();

		// Get collision.
		bool collision = scene.intersect(shadow_ray, 0.0001, distance_to_light, rec);

		// If the light is obscured, continue.
		if (collision == true) {
			continue;
		}
		
		pixel_lighting += light->illuminate(position, normal, direction);
	}
	return pixel_lighting;
}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void MyApp::Init()
{
	// Instantiate colored material pointers.
	shared_ptr<DiffuseMaterial> purple_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
	shared_ptr<DiffuseMaterial> pink_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.9f, 0.3f, 0.4f)));
	shared_ptr<DiffuseMaterial> lavender_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.4f, 0.4f, 0.8f)));
	shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.85f, 0.85f, 0.9f)));

	// Special materials
	shared_ptr<NormalMaterial> normal_mat = make_shared<NormalMaterial>(NormalMaterial());
	shared_ptr<CheckerboardMaterial> plane_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(0.4f,0.2f,1.f), float3(0.4f, 1.f,0.7f)));
	shared_ptr<MirrorMaterial> mirror_mat = make_shared<MirrorMaterial>(MirrorMaterial());

	// Instantiate object pointers.
	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(2, 1, 4), 2, normal_mat));
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-2, 1, 4), 1, mirror_mat));
	shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -3, 0), float3(0, 1, 0), plane_mat));
	shared_ptr<Plane> left_wall = make_shared<Plane>(Plane(float3(-6, 0, 0), float3(1, 0, 0), pink_mat));
	shared_ptr<Plane> back_wall = make_shared<Plane>(Plane(float3(0, 0, 8), float3(0, 0, -1), off_white_mat));
	shared_ptr<Plane> right_wall = make_shared<Plane>(Plane(float3(6, 0, 0), float3(-1, 0, 0), lavender_mat));
	shared_ptr<Plane> ceiling = make_shared<Plane>(Plane(float3(0, 6, 0), float3(0, -1, 0), off_white_mat));

	// Put all scene objects in a list
	list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ left_wall, right_wall, back_wall, ceiling, sphere1, sphere2, floor});

	shared_ptr<PointLight> point_light = make_shared<PointLight>(PointLight(float3(0, 5, 4), 0.66f));
	shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(0.34f));

	list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ point_light, ambient_light });

	scene = Scene(objects, lights);
	camera = Camera();
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void MyApp::Tick( float deltaTime )
{
	// clear the screen to black
	//screen->Clear( 0 );

	float3 x_dir = camera.screen_p1 - camera.screen_p0;
	float3 y_dir = camera.screen_p2 - camera.screen_p0;
	
	if (key_held_down)
	{
		camera.keyHandler(held_key);
	}

	// Loop over every pixel in the screen.
	for (int x = 0; x < SCRWIDTH; x++) for (int y = 0; y < SCRHEIGHT; y++) {

		// 	Point on the screen:
		// 𝑃(𝑢, 𝑣) = 𝑃0 + 𝑢(𝑃1 −𝑃0) + 𝑣(𝑃2 −𝑃0)
	    // 𝑢, 𝑣 ∈[0, 1]
		float u = x / (SCRWIDTH - 1.f);
		float v = y / (SCRHEIGHT - 1.f);

		float3 screen_point = camera.screen_p0 + u * x_dir + v * y_dir;
		
		// Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
		float3 ray_dir = normalize(screen_point - camera.cameraPos);

		Ray ray = Ray(camera.cameraPos, ray_dir);

		float3 pixel_color = Trace(ray);

		uint c = translate_color(pixel_color);

		screen->Plot(x, y, c);
	}
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
	}
}
void MyApp::MouseDown(int button)
{
	if (button == 0) 
	{
		mouse_held_down = true;
	}	
}