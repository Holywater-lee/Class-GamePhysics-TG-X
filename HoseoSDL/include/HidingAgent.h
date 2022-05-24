#pragma once

#include "Walker.h"
class Obstacle;

class HidingAgent : public Walker
{
public:
	HidingAgent(float x, float y);
	~HidingAgent() {}

	void draw(SDL_Renderer* renderer) override;

	Vector2D Hide(Walker* hunter, const std::vector<Obstacle*>& obstacles);

private:
	Vector2D GetHidingPos(const Vector2D& obst_pos, const float obst_radius, const Vector2D& hunter_Pos);
};