#include "HidingAgent.h"
#include "Obstacle.h"

#define MAX_DIST 10000000.0f

HidingAgent::HidingAgent(float x, float y) : Walker(x, y)
{

}

Vector2D HidingAgent::Hide(Walker* hunter, const std::vector<Obstacle*>& obstacles)
{
	float closestDistance = MAX_DIST;
	Vector2D bestHidingSpot = location;
	
	std::vector<Obstacle*>::const_iterator cur_obst = obstacles.begin();
	std::vector<Obstacle*>::const_iterator closest_obst;

	while (cur_obst != obstacles.end())
	{
		Vector2D HidingSpot = GetHidingPos((*cur_obst)->position, (*cur_obst)->radius, hunter->getLocation());

		float distance = Vector2D(HidingSpot - location).lengthSqaured();

		if (distance < closestDistance)
		{
			closestDistance = distance;
			bestHidingSpot = HidingSpot;
			closest_obst = cur_obst;
		}

		++cur_obst;
	}

	if (closestDistance == MAX_DIST) return pursue(hunter, true);

	return Arrive(bestHidingSpot);
}

Vector2D HidingAgent::GetHidingPos(const Vector2D& obst_pos, const float obst_radius, const Vector2D& hunter_Pos)
{
	float DistanceFromBoundary = 30.0f;
	float DistAway = obst_radius + DistanceFromBoundary;

	Vector2D ToObs = Vector2D(obst_pos + hunter_Pos).Normalized();

	return obst_pos + (ToObs * DistAway);
}