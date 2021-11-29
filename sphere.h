#pragma once

// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
// TODO: Material.
class Sphere
{
public:
	float3 position = float3(0, 0, 0);
	float radius = 1.f;

	Sphere(float3 position) {
		this->position = position;
		cout << "Initializing Sphere!";
	}
};

