#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Vector2D.h"

#include <vector>

#include <queue>

class Walker;
class Obstacle;
class HidingAgent;

class Game {
private:
	Game() : mousePos(0, 0) {}
	static Game* s_pInstance;
public:
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}

	SDL_Renderer* getRenderer() const { return m_pRenderer; }

public:
	~Game() {}

	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool setup();
	void render();
	void update();
	bool running();
	void handleEvents();
	void clean();
	void quit() { m_bRunning = false; }

	void InsertDrawQueue(float radius, Vector2D pos, bool isRed)
	{
		CircleToDraw circle_to_push(radius, pos, isRed);
		circlesToDraw.push(circle_to_push);
	}

private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	bool m_bRunning;

	int numHidingAgents;
	std::vector<HidingAgent*> hiders;
	Walker* hunter;

	int numObstacles;
	std::vector<Obstacle*> obstacles;

	struct CircleToDraw
	{
		CircleToDraw(float _radius, Vector2D _pos, bool _isRed) : radius(_radius), pos(_pos), isRed(_isRed) {}
		float radius;
		Vector2D pos;
		bool isRed;
	};

	std::queue<CircleToDraw> circlesToDraw;

	Vector2D mousePos;
};

typedef Game TheGame;