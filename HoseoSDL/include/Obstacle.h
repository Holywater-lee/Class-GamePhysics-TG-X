#pragma once

#include "Vector2D.h"

class Obstacle
{
public:
	Obstacle(float x, float y, float r) : position(x, y), radius(r) {}

	Vector2D position;
	float radius;
};