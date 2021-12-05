#include "precomp.h"
#include "myapp.h"

TheApp* CreateApp() { return new MyApp(); }

#include "scene.h"
#include "color.h"

using namespace RaytracingRenderer;

float3 MyApp::rayColor(Ray ray) {
	auto t = 0.5f * (ray.dir.y + 1);
	float3 pixel_color = (1 - t) * float3(1, 1, 1) + t * float3(0.5f, 0.7f, 1);
	return pixel_color;
}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void MyApp::Init()
{
	// anything that happens only once at application start goes here
	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(2, 0, 4), 2, Material(float3(0, 1, 0))));
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-2, -1, 4), 1, Material(float3(1, 0, 0))));
	shared_ptr<Plane> plane = make_shared<Plane>(Plane(float3(0, -1, 0), float3(0, 1, 0), Material(float3(0, 0, 1))));
	list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>();

	objects.push_back(sphere1);
	objects.push_back(plane);
	objects.push_back(sphere2);
	scene = Scene(objects);
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void MyApp::Tick( float deltaTime )
{
	// clear the screen to black
	screen->Clear( 0 );
	// print something to the console window
	//printf( "hello world!\n" );

	Camera camera = scene.camera;
	float3 x_dir = camera.screen_p1 - camera.screen_p0;
	float3 y_dir = camera.screen_p2 - camera.screen_p0;
	
	if (key_held_down)
	{
		scene.camera.keyHandler(held_key);
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
		float3 ray_dir = normalize(screen_point - camera.origin);

		Ray ray = Ray(camera.origin, ray_dir);
		hit_record rec = hit_record();

		const bool collision = scene.intersect(ray, 0.0001f, FLT_MAX, rec);

		uint c;

		// If no collision was found for this ray, draw a nice BG color.
		if (collision == false) {
			// Create a nice background color.
			float3 pixel_color = rayColor(ray);
			c = translate_color(pixel_color);
		}
		// If a collision was found, get the color of the object.
		else {
			// TODO: get object color at position.
			float3 pixel_color = rec.mat_ptr->diffuseColor;
			c = translate_color(pixel_color);
		}
		
		screen->Plot(x, y, c);
	}

	
	//// plot some colors
	//for( int red = 0; red < 256; red++ ) for( int green = 0; green < 256; green++ )
	//{
	//	int x = red, y = green;
	//	screen->Plot( x + 200, y + 100, (red << 16) + (green << 8) );
	//}
	//// plot a white pixel in the bottom right corner
	//screen->Plot( SCRWIDTH - 2, SCRHEIGHT - 2, 0xffffff );
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
		scene.camera.mouseHandler(x, y);
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