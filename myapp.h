#pragma once
using RaytracingRenderer::Ray;
using RaytracingRenderer::hit_record;
#include <mutex>

namespace Tmpl8
{

class MyApp : public TheApp
{
public:
	struct Blockjob
	{
		int rowStart;
		int rowEnd;
		int colSize;
		int spp;

		vector<float2> indices;
		vector<float3> colors;
	};
	const int nThreads = thread::hardware_concurrency();
	int rowsPerThread = SCRHEIGHT / nThreads;
	int leftOver = SCRHEIGHT % nThreads;

	mutex mute;
	condition_variable cvResults;
	vector<Blockjob> imageBlocks;
	atomic<int> completedThreads = { 0 };
	vector<thread> threads;

	int max_bounces = 10;
	int max_reflection_shadows = 4;
	bool antialiasing = true;
	bool multithreading = true;
	//EDIT THIS TO CHANGE ANTI-ALIASING STRENGTH. 100 is beautiful but slow. 0 is none. 
	int samples_per_pixel = 10;

	// game flow methods
	void Init();
	void Tick( float deltaTime );
	void CalculateColor(Blockjob job);
	float3 BackgroundColor(Ray& ray, float3& pixel_color);
	float3 Trace(Ray &ray);
	float3 TraceSolid(Ray& ray, hit_record& rec, uint bounce_count);
	float3 TraceGlass(Ray& ray, hit_record& rec, uint bounce_count);
	float3 DirectIllumination(float3 &position, float3 &normal);
	float3 TraceReflection(Ray& ray, uint bounce_count);
	float3 TraceRefraction(Ray& ray, uint bounce_count);
	

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
	RaytracingRenderer::Camera camera;
	MyApp() {};
};

} // namespace Tmpl8