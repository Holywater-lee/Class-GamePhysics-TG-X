#pragma once
#include <random>
#include <Vector2D.h>

class MyRandom
{
public:
	// 두 범위 사이의 랜덤 float 값 구하기
	static float GetRandomFloat(float range1, float range2)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> getrand(range1, range2);
		return getrand(gen);
	}

	// 0 ~ range 사이 float 값 구하기
	static float GetRandomFloat(float range)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> getrand2(0, range);
		return getrand2(gen);
	}

	static Vector2D Random2D()
	{
		return Vector2D(GetRandomFloat(1), GetRandomFloat(1));
	}
};