#include "Target.h"
#include <MyRandom.h>

const int WIDTH = 600;
const int HEIGHT = 400;

Target::Target(float x, float y) : Walker(x, y)
{
	init(Vector2D(x, y));
}

void Target::init(Vector2D pos)
{
	refreshLocation(pos);
	maxSpeed = 4;
	velocity = MyRandom::Random2D().Normalized() * maxSpeed;
}

void Target::draw(SDL_Renderer* renderer)
{
	filledCircleRGBA(renderer, location.getX(), location.getY(), r * 2, 200, 200, 50, 100);
	aacircleRGBA(renderer, location.getX(), location.getY(), r * 2, 200, 200, 50, 255);
}

void Target::drawPrediction(SDL_Renderer* renderer, Vector2D prediction)
{
	//Vector2D vertex = Vector2D(pos.length(), 0).RotTranslate(heading, location.getX(), location.getY());
	//lineRGBA(renderer, vertex.getX(), vertex.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	//filledCircleRGBA(renderer, vertex.getX(), vertex.getY(), r, 155, 255, 255, 100);
	//aacircleRGBA(renderer, vertex.getX(), vertex.getY(), r, 155, 255, 255, 255);
	Vector2D predictedPos = prediction + location;
	lineRGBA(renderer, predictedPos.getX(), predictedPos.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	filledCircleRGBA(renderer, predictedPos.getX(), predictedPos.getY(), r, 155, 255, 255, 100);
	aacircleRGBA(renderer, predictedPos.getX(), predictedPos.getY(), r, 155, 255, 255, 255);
}

void Target::edges()
{
	float buffer = r * 2;

	if (location.getX() > WIDTH - buffer)
	{
		velocity.setX(velocity.getX() * -1);
		location.setX(WIDTH - buffer);
	}
	else if (location.getX() < buffer)
	{
		velocity.setX(velocity.getX() * -1);
		location.setX(buffer);
	}

	if (location.getY() > HEIGHT - buffer)
	{
		velocity.setY(velocity.getY() * -1);
		location.setY(HEIGHT - buffer);
	}
	else if (location.getY() < buffer)
	{
		velocity.setY(velocity.getY() * -1);
		location.setY(buffer);
	}
}