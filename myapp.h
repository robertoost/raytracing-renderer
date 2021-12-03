#pragma once
#include "scene.h"
#include "ray.h"

namespace Tmpl8
{

class MyApp : public TheApp
{
public:
	// game flow methods
	void Init();
	void Tick( float deltaTime );
	float3 rayColor(RaytracingRenderer::Ray ray);
	void Shutdown() { /* implement if you want to do something on exit */ }
	// input handling
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove(int x, int y);
	void KeyUp(int key);
	void KeyDown(int key);
	// data members
	//int2 mousePos = (0, 0);
	RaytracingRenderer::Scene scene;
	MyApp() {};
};

} // namespace Tmpl8