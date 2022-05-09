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

	_walker = new Walker(50, 50);
	_target = new Target(WIDTH - 50, HEIGHT - 50);

	return result;
}

void Game::update()
{
	_walker->applyForce(_walker->pursue(_target->getLocation()));

	float dist = Vector2D(_walker->getLocation() - _target->getLocation()).length();
	if (dist < _walker->getR() + _target->getR())
	{
		_target->init(Vector2D(MyRandom::GetRandomFloat(WIDTH), MyRandom::GetRandomFloat(HEIGHT)));
		_walker->refreshLocation(Vector2D(WIDTH / 2, HEIGHT / 2));
	}

	_walker->update();
	_target->update();

	_walker->edges();
	_target->edges();
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	_walker->draw(m_pRenderer);
	_target->draw(m_pRenderer);
	_target->drawPrediction(m_pRenderer, _walker->pursue(_target->getLocation()));

	SDL_RenderPresent(m_pRenderer);
}

void Game::clean()
{
	delete _walker;
	delete _target;

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