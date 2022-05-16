const int WIDTH = 600;
const int HEIGHT = 400;

#include "Game.h"
#include "InputHandler.h"

#include "Target.h"
#include "Walker.h"
#include "MyRandom.h"

Game* Game::s_pInstance = 0;

bool Game::setup()
{
	bool result;

	result = init("Nature of Code", 100, 100, WIDTH, HEIGHT, false);
	
	numWalkers = 1;
	_walkers.reserve(numWalkers);
	for (int i = 0; i < numWalkers; i++)
	{
		_walkers.push_back(new Walker(50 + 10 * i, 50 + 10 * i));
	}

	return result;
}

void Game::update()
{
	for (const auto& w : _walkers)
	{
		//w->applyForce(w->pursue(_target));
		w->Wander();
		w->applyForce(Vector2D(1, 0));
		w->update();
		w->edges();
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
	
	for (const auto& w : _walkers)
	{
		w->draw(m_pRenderer);
	}

	//_target->draw(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
	for (int i = 0; i != _walkers.size(); i++)
	{
		delete _walkers[i];
	}
	_walkers.clear();

	//delete _target;

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