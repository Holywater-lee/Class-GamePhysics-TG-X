#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Vector2D.h"
#include "MyRandom.h"

class Walker
{
public:
	Walker(float x, float y);
	virtual ~Walker() {}
	virtual void draw(SDL_Renderer* renderer);
	void update();
	void applyForce(const Vector2D& force);
	Vector2D seek(const Vector2D& target, bool flee = false);
	Vector2D pursue(Walker* target, bool evade = false);
	void Wander();
	Vector2D Arrive(const Vector2D& target);
	virtual void edges();
	void refreshLocation(const Vector2D& pos) { location = pos; }

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

	float arriveDeceleration = 2;

	float theta = 3.1415926535 * 2 * MyRandom::GetRandomFloat(0, 1);

	Vector2D m_finalTarget;
	Vector2D m_vWanderPoint;
	float m_fWanderRadius = 40;
	float m_fWanderDistance = 100;
};