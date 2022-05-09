#pragma once

#include "Walker.h"

class Target : public Walker
{
public:
	Target(float x, float y);
	void draw(SDL_Renderer* renderer) override;
	void edges() override;
	void drawPrediction(SDL_Renderer* renderer, Vector2D prediction);

	void init(Vector2D pos);
};