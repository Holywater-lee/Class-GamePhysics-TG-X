const int WIDTH = 600;
const int HEIGHT = 400;

#include "Game.h"
#include "InputHandler.h"

#include "Walker.h"
#include "HidingAgent.h"
#include "Obstacle.h"
#include "MyRandom.h"

Game* Game::s_pInstance = 0;

bool Game::setup()
{
	bool result;

	result = init("Nature of Code", 100, 100, WIDTH, HEIGHT, false);

	numObstacles = 5;
	obstacles.reserve(numObstacles);
	for (int i = 0; i < numObstacles; i++)
	{
		obstacles.push_back(new Obstacle(
							MyRandom::GetRandomFloat(50, WIDTH - 50),
							MyRandom::GetRandomFloat(50, HEIGHT - 50),
							MyRandom::GetRandomFloat(20, 30)));
	}
	
	numHidingAgents = 4;
	hiders.reserve(numHidingAgents);
	for (int i = 0; i < numHidingAgents; i++)
	{
		hiders.push_back(new HidingAgent(MyRandom::GetRandomFloat(50, WIDTH - 50),
			MyRandom::GetRandomFloat(50, HEIGHT - 50)));
	}

	hunter = new Walker(WIDTH / 2, HEIGHT / 2);

	return result;
}

void Game::update()
{
	mousePos = *InputHandler::Instance()->getMousePosition();
	for (const auto& h : hiders)
	{
		h->applyForce(h->Hide(hunter, obstacles));
		h->update();
		h->edges();
	}

	hunter->applyForce(hunter->Arrive(mousePos));
	hunter->update();
	hunter->edges();
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
	
	filledCircleRGBA(m_pRenderer, mousePos.getX(), mousePos.getY(), 16, 255, 0, 0, 255);

	for (const auto& o : obstacles)
	{
		o->draw(m_pRenderer);
	}

	for (const auto& h : hiders)
	{
		h->draw(m_pRenderer);
	}

	hunter->draw(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
	for (int i = 0; i != hiders.size(); i++)
	{
		delete hiders[i];
	}
	hiders.clear();

	delete hunter;

	TheInputHandler::Instance()->clean();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);

	m_pWindow = NULL;
	m_pRenderer = NULL;

	SDL_Quit();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);

	if (m_pWindow != NULL)
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(m_pRenderer);
	}
	else
	{
		return false;
	}

	m_bRunning = true;

	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

	return true;
}

bool Game::running()
{
	return m_bRunning;
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}