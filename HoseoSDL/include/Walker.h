#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Vector2D.h"

class Walker
{
public:
	Walker(float x, float y);
	virtual void draw(SDL_Renderer* renderer);
	void update();
	void applyForce(Vector2D force);
	Vector2D seek(Vector2D target, bool flee = false);
	Vector2D pursue(Walker* target, bool evade = false);
	virtual void edges();
	void refreshLocation(Vector2D pos) { location = pos; }

	Vector2D getLocation() const { return location; }
	Vector2D getVelocity() const { return velocity; }
	int getR() const { return r; }

protected:
	Vector2D acceleration;
	Vector2D velocity;
	Vector2D location;
	const int r;
	const float maxForce;
	float maxSpeed;
	float heading = 0;
};