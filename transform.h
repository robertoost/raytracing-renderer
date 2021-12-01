#pragma once
class Transform
{
public: 
	float3 position;
	// TODO: ROTATION, SCALE
	Transform(float3 position) {
		this->position = position;
	}
};

