#include "Walker.h"
#include "Game.h"
#include "Target.h"

const int WIDTH = 600;
const int HEIGHT = 400;

Walker::Walker(float x, float y) : location(x, y), velocity(0, 0), acceleration(0, 0), r(16), maxForce(0.2), m_vWanderPoint(0, 0), m_finalTarget(0, 0)
{
	maxSpeed = 4;
}

void Walker::update()
{
	velocity += acceleration;
	velocity.limit(maxSpeed);
	location += velocity;
	acceleration *= 0;

	heading = atan2(velocity.getY(), velocity.getX());
}

Vector2D Walker::seek(Vector2D target, bool flee)
{
	Vector2D desiredForce = target - location;

	desiredForce = desiredForce.Normalized() * maxSpeed - velocity;
	if (flee) desiredForce *= -1;

	desiredForce.limit(maxForce);

	return desiredForce;
}

Vector2D Walker::pursue(Walker* target, bool evade)
{
	Vector2D prediction = target->getVelocity();

	float dist = Vector2D(target->getLocation() - location).length();

	prediction = prediction * dist / 30;
	dynamic_cast<Target*>(target)->predictionLength = prediction.length();

	return seek(target->getLocation() + prediction) * (evade ? -1 : 1);
}

void Walker::Wander()
{
	Vector2D m_vWanderTarget = Vector2D(m_fWanderRadius * cos(theta), m_fWanderRadius * sin(theta));
	m_vWanderTarget += Vector2D(MyRandom::GetRandomFloat(-1, 1) * 20, MyRandom::GetRandomFloat(-1, 1) * 20);
	m_vWanderTarget = m_vWanderTarget.Normalized() * m_fWanderRadius;

	m_finalTarget = m_vWanderTarget + Vector2D(m_fWanderDistance, 0);
	m_finalTarget = m_finalTarget.RotTranslate(heading, location);

	m_vWanderPoint = velocity;
	m_vWanderPoint = m_vWanderPoint.Normalized() * m_fWanderDistance;
	m_vWanderPoint = m_vWanderPoint.RotTranslate(heading, location);
}

void Walker::applyForce(Vector2D force)
{
	acceleration += force;
}

void Walker::edges()
{
	float buffer = r;
	if (location.getX() > WIDTH + buffer) location.setX(-buffer);
	else if (location.getX() < -buffer) location.setX(WIDTH + buffer);
	if (location.getY() > HEIGHT + buffer) location.setY(-buffer);
	else if (location.getY() < -buffer) location.setY(HEIGHT + buffer);
}

void Walker::draw(SDL_Renderer* renderer)
{
	Vector2D vertex0 = Vector2D(r, 0).RotTranslate(heading, location.getX(), location.getY());
	Vector2D vertex1 = Vector2D(-r, -r / 2).RotTranslate(heading, location.getX(), location.getY());
	Vector2D vertex2 = Vector2D(-r, r / 2).RotTranslate(heading, location.getX(), location.getY());

	filledTrigonRGBA(renderer, vertex0.getX(), vertex0.getY(), vertex1.getX(), vertex1.getY(), vertex2.getX(), vertex2.getY(), 125, 125, 125, 255);
	aatrigonRGBA(renderer, vertex0.getX(), vertex0.getY(), vertex1.getX(), vertex1.getY(), vertex2.getX(), vertex2.getY(), 255, 255, 255, 255);

	lineRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	lineRGBA(renderer, m_finalTarget.getX(), m_finalTarget.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	aacircleRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), m_fWanderRadius, 255, 255, 255, 255);
	filledCircleRGBA(renderer, m_finalTarget.getX(), m_finalTarget.getY(), 4, 100, 255, 100, 255);
	filledCircleRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), 4, 255, 0, 0, 255);
}
