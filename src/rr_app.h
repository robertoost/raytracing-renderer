#pragma once
using RaytracingRenderer::Scene;
using RaytracingRenderer::Camera;
using RaytracingRenderer::Renderer;

namespace Tmpl8
{

class RRApp : public TheApp
{
public:
	// game flow methods
	void Init();
	void Tick( float deltaTime );	
	void Shutdown() { delete(this->renderer); }
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
	Scene scene;
	Camera camera;
	Renderer * renderer = nullptr;
	RRApp() {};
};

} // namespace Tmpl8