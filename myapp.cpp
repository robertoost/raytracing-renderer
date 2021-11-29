#include "precomp.h"
#include "myapp.h"

TheApp* CreateApp() { return new MyApp(); }



// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void MyApp::Init()
{
	// anything that happens only once at application start goes here

	// Camera setup:

	float d = 4.f;

	float3 camPos = float3(0, 0, 0); 	// Camera position: 𝐸 =(0,0,0)
	float3 viewDir = float3(0, 0, 1); 	// View direction : 𝑉 = (0, 0, 1)
	float3 screenCenter = camPos + d * viewDir; 	// Screen center : 𝐶 = 𝐸 + 𝑑𝑉
		// Screen corners : 𝑃0 = 𝐶 + −1, −1, 0, 𝑃1 = 𝐶 + 1, −1, 0, 𝑃2 = 𝐶 + (−1, 1, 0)

	// Change FOV by altering 𝑑;
	// Transform camera by multiplying E, 𝑃0, 𝑃1, 𝑃2 with a camera matrix.

	// TODO: C++ how do I get this to the tick function?
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void MyApp::Tick( float deltaTime )
{
	// clear the screen to black
	screen->Clear( 0 );
	// print something to the console window
	printf( "hello world!\n" );
	
	// plot some colors
	for( int red = 0; red < 256; red++ ) for( int green = 0; green < 256; green++ )
	{
		int x = red, y = green;
		screen->Plot( x + 200, y + 100, (red << 16) + (green << 8) );
	}
	// plot a white pixel in the bottom right corner
	screen->Plot( SCRWIDTH - 2, SCRHEIGHT - 2, 0xffffff );
}