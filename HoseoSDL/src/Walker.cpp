#include "Walker.h"
#include "Game.h"
//#include "Target.h"
#include "Obstacle.h"

const int WIDTH = 600;
const int HEIGHT = 400;

inline float Clamp(float src, float min, float max)
{
	src = src > max ? max : src;
	return src < min ? min : src;
}

Walker::Walker(float x, float y) : location(x, y), velocity(0, 1), acceleration(0, 0), r(16), maxForce(0.2), m_vWanderPoint(0, 0), m_finalTarget(0, 0)
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

Vector2D Walker::ObstacleAvoidance(const std::vector<Obstacle*>& obstacles)
{
	// 감지상자 길이 조정
	m_dDBoxLength = MinDetectionBoxLength + (velocity.length() / maxSpeed) * MinDetectionBoxLength;

	Obstacle* ClosestObstacle = nullptr;
	Vector2D ClosestObstacleLocalPos(0, 0);
	float DistToClosestObst = 1000000.0f;
	std::vector<Obstacle*>::const_iterator curObstIter = obstacles.begin();

	while (curObstIter != obstacles.end())
	{
		// 1. 감지상자 범위 내인지 확인
		if ((location - (*curObstIter)->position).lengthSqaured() < m_dDBoxLength * m_dDBoxLength)
		{
			Vector2D LocalObstPos = (*curObstIter)->IntoLocalSpace(location, heading);

			// 디버그용!!!
			TheGame::Instance()->InsertDrawQueue((*curObstIter)->radius, LocalObstPos + Vector2D(300, 200), false);

			// 2. 전방의 장애물만 고려
			if (LocalObstPos.getX() >= 0)
			{
				// 3. 장애물이 감지상자와 겹치는지 확인
				// 범위 확장
				float ExpandedRadius = (*curObstIter)->radius + r;

				if (abs(LocalObstPos.getY()) < ExpandedRadius)
				{
					float cX = LocalObstPos.getX();
					float cY = LocalObstPos.getY();

					// 4. 운반기에 가장 가까운 교차점 찾기
					// 피타고라스 역으로 사용하여 닿은 두 점 중 가까운 점 찾기
					float sqrtPart = sqrt(ExpandedRadius * ExpandedRadius - cY * cY);
					float selectedPoint = cX - sqrtPart;

					if (selectedPoint <= 0) selectedPoint = cX + sqrtPart;

					// 가장 가까운 점 선택하고 정보 저장하기
					if (selectedPoint < DistToClosestObst)
					{
						DistToClosestObst = selectedPoint;
						ClosestObstacle = *curObstIter;
						ClosestObstacleLocalPos = LocalObstPos;
					}
				}
			}
		}

		++curObstIter;
	}

	// 5. 조정힘 계산하기
	Vector2D Steering(0, 0);

	// 이 if문 안걸면 오류남 (탐지하지 못했는데 nullptr 참조하려 함)
	if (ClosestObstacle)
	{
		// 가까이 있을수록 커지는 값 계산하기
		float multiplier = 1.0 + (m_dDBoxLength - ClosestObstacleLocalPos.getX()) / m_dDBoxLength;

		// 옆방향 조정 힘 계산하기. X축에 더 깊게 파고들어있을수록 강한 힘 계산
		Steering.setY((ClosestObstacle->radius - abs(ClosestObstacleLocalPos.getY())) * multiplier);

		const float BrakingWeight = 0.2;
		// 제동력 계산하기. 멀리서부터 점차 제동하면서 왔기에 가까이 갈수록 덜 제동한다.
		Steering.setX((ClosestObstacle->radius - ClosestObstacleLocalPos.getX()) * BrakingWeight);
	}
	
	return Steering.RotateAngle(heading);
}

Vector2D Walker::seek(const Vector2D& target, bool flee)
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
	//dynamic_cast<Target*>(target)->predictionLength = prediction.length();

	return seek(target->getLocation() + prediction) * (evade ? -1 : 1);
}

void Walker::Wander()
{
	// 배회 경계원 위의 타겟
	Vector2D m_vWanderTarget = Vector2D(m_fWanderRadius * cos(theta), m_fWanderRadius * sin(theta));

	// 타겟의 무작위 떨림 움직임 및 원 위로 재투사
	m_vWanderTarget += Vector2D(MyRandom::GetRandomFloat(-1, 1) * 20, MyRandom::GetRandomFloat(-1, 1) * 20);
	m_vWanderTarget = m_vWanderTarget.Normalized() * m_fWanderRadius;

	// 배회 경계원 중앙
	m_vWanderPoint = velocity;
	m_vWanderPoint = m_vWanderPoint.Normalized() * m_fWanderDistance;
	m_vWanderPoint += location;

	// m_fWanderDistance만큼 앞으로 이동
	m_finalTarget = m_vWanderTarget + Vector2D(m_fWanderDistance, 0);
	m_finalTarget = m_finalTarget.RotTranslate(heading, location);

	Vector2D steer = m_finalTarget - location;
	steer = steer.Normalized() * maxForce;
	applyForce(steer);
}

Vector2D Walker::Arrive(const Vector2D& target)
{
	Vector2D Direction = target - location;
	float Distance = Direction.length();

	if (Distance > 0)
	{
		float speed = Distance / arriveDeceleration;

		speed = Clamp(speed, speed, maxSpeed);

		Vector2D DesiredVector = Direction / Distance * speed;

		return DesiredVector - velocity;
	}

	return Vector2D(0, 0);
}

void Walker::applyForce(const Vector2D& force)
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

	filledTrigonRGBA(renderer, vertex0.getX(), vertex0.getY(), vertex1.getX(), vertex1.getY(), vertex2.getX(), vertex2.getY(), 255, 255, 255, 200);
	aatrigonRGBA(renderer, vertex0.getX(), vertex0.getY(), vertex1.getX(), vertex1.getY(), vertex2.getX(), vertex2.getY(), 255, 255, 255, 255);

	/*
	lineRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	lineRGBA(renderer, m_finalTarget.getX(), m_finalTarget.getY(), location.getX(), location.getY(), 255, 255, 255, 255);
	aacircleRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), m_fWanderRadius, 255, 255, 255, 255);

	filledCircleRGBA(renderer, m_vWanderPoint.getX(), m_vWanderPoint.getY(), 4, 255, 0, 0, 255);

	filledCircleRGBA(renderer, m_finalTarget.getX(), m_finalTarget.getY(), 12, 100, 255, 100, 200);
	aacircleRGBA(renderer, m_finalTarget.getX(), m_finalTarget.getY(), 12, 100, 255, 100, 255);
	*/
}
