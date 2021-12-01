#include "precomp.h"
#include "myapp.h"

TheApp* CreateApp() { return new MyApp(); }


#include "scene.h"
#include <vec3.h>
#include <color.h>
using namespace RaytracingRenderer;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void MyApp::Init()
{
	// anything that happens only once at application start goes here
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
	Sphere sphere = scene.sphere;

	vec3 xDir = camera.screenP1 - camera.screenP0;
	vec3 yDir = camera.screenP2 - camera.screenP0;

	// Loop over every pixel in the screen.
	for (int x = 0; x < SCRWIDTH; x++) for (int y = 0; y < SCRHEIGHT; y++) {

		// 	Point on the screen:
		// 𝑃(𝑢, 𝑣) = 𝑃0 + 𝑢(𝑃1 −𝑃0) + 𝑣(𝑃2 −𝑃0)
	    // 𝑢, 𝑣 ∈[0, 1]
		double u = x / (SCRWIDTH - 1.);
		double v = y / (SCRHEIGHT - 1.);

		vec3 screenPoint = camera.screenP0 + u * xDir + v * yDir;
		
		/* Ray direction (normalized):
				  𝑃 𝑢,𝑣 −𝐸
			𝐷 = _____________
				∥𝑃 𝑢,𝑣 −𝐸 ∥   */
		vec3 rayDir = screenPoint - camera.camPos;

		// Normalize by dividing by the magnitude.
	    // Magnitude from https://stackoverflow.com/questions/48306941/efficient-magnitude-calculation-of-3d-vector
		rayDir = rayDir / rayDir.length();
		
		// Create a nice background color.
		auto t = 0.5 * (rayDir.y() + 1.);
		color pixel_color = (1. - t)* color(1., 1., 1.) + t * color(0.5, 0.7, 1.);
		uint c = translate_color(pixel_color);

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


// TODO: Control camera with keyboard. (and mouse?)
//void MyApp::KeyUp(int key)
//{
//	/* implement if you want to handle keys */
//}
//void MyApp::KeyDown(int key)
//{
//	/* implement if you want to handle keys */
//}