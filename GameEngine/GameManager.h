#pragma once
#include <SDL/SDL.h>
#include "Vec2D.h"
#include "GameState.h"
#include "InputManager.h"
#include "RenderingManager.h"
class GameManager
{
public:
	GameManager();
	~GameManager();

	void runGame();
private:
	void initSystems();
	void gameLoop();

	Vec2D <int> _windowDims;
	SDL_Window* _window;
	InputManager* _inputManager;
	RenderingManager* _renderingManager;
	GameState _gameState;

	GLuint _time;
};

