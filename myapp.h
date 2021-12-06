﻿#pragma once

namespace Tmpl8
{

class MyApp : public TheApp
{
public:
	// game flow methods
	void Init();
	void Tick( float deltaTime );
	float3 Trace(RaytracingRenderer::Ray ray);
	float3 DirectIllumination(float3 &position, float3 &normal);
	void Shutdown() { /* implement if you want to do something on exit */ }
	// input handling
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove(int x, int y);
	void KeyUp(int key);
	void KeyDown(int key);
	// data members
	//int2 mousePos = (0, 0);
	bool mouse_held_down = false;
	bool key_held_down = false;
	int held_key = 0;
	RaytracingRenderer::Scene scene;
	MyApp() {};
};

} // namespace Tmpl8