#pragma once

namespace RaytracingRenderer {

class SceneManager
{
public:
	static Scene CornellBox();

	static Scene GlassTest();

	static Scene BeersLaw();

	static Scene DirectionalLightTest();

	static Scene Snowman();

	static Scene TriangleTest();

	//static Scene MeshTest();

	static Scene SingleObject();

	static Scene TooManySpheres();
	
	static Scene AreaLightTest();

	static Scene IndirectLightTest();
};
}

