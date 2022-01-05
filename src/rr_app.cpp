#include "precomp.h"
#include "rr_app.h"


// Raynetracing Renderobert App. (2021-12-15)
TheApp* CreateApp() { return new RRApp(); }

using namespace RaytracingRenderer;

float3 frame[SCRHEIGHT][SCRWIDTH];

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void RRApp::Init()
{
	scene = SceneManager::TriangleTest();
	raytracer = new Raytracer(scene, camera);
	//cout << '\n' << "After init " << raytracer.scene.objects.size() << '\n';
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void RRApp::Tick( float deltaTime )
{
	Timer myTimer;
	myTimer.reset();
	// clear the screen to black
	screen->Clear( 0 );


	float3 x_dir = camera.screen_p1 - camera.screen_p0;
	float3 y_dir = camera.screen_p2 - camera.screen_p0;

	float x_width = length(x_dir);
	float y_height = length(y_dir);

	if (key_held_down)
	{
		camera.keyHandler(held_key);
	}

	raytracer->RenderScene(frame);

	if (POSTPROCESSING) {
		if (CHROMATIC_ABERRATION) {
			PostProcessing::chromaticAberration(frame, CHROM_ABB_R_OFFSET, CHROM_ABB_G_OFFSET, CHROM_ABB_B_OFFSET);
		}
		if (VIGNETTING) {
			PostProcessing::vignetting(frame, float2(SCRHEIGHT / 2.f, SCRWIDTH / 2.f));
		}
		if (GAMMA_CORRECTION) {
			PostProcessing::gammaCorrection(frame, GAMMA);
		}
	}

	// Loop over every pixel in the screen
	for (int y = 0; y < SCRHEIGHT; y++) {
		for (int x = 0; x < SCRWIDTH; x++) {
			uint c = translate_color(frame[y][x]);
			screen->Plot(x, y, c);
		}
	}
	
	//cout << "done";
	cout << myTimer.elapsed() * 1000 << " ";
}

void RRApp::KeyUp(int key)
{
	key_held_down = false;
}
void RRApp::KeyDown(int key)
{
	key_held_down = true;
	held_key = key;
}
void RRApp::MouseMove(int x, int y) 
{
	if (mouse_held_down)
	{
		camera.mouseHandler(x, y);
	}
}
void RRApp::MouseUp(int button)
{
	if (button == 0)
	{
		mouse_held_down = false;
		camera.firstMouse = true;
	}
}
void RRApp::MouseDown(int button)
{
	if (button == 0) 
	{
		mouse_held_down = true;
	}	
}