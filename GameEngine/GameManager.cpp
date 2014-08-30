#include "GameManager.h"
#include <Windows.h>
#include <GL/glew.h>
#include "ErrorManager.h"

GameManager::GameManager()
	: _window(nullptr), _inputManager(new InputManager()), _renderingManager(nullptr), _time(0)
{
}

GameManager::~GameManager()
{
}

void GameManager::runGame() {
	initSystems();
	
	gameLoop();
}

void GameManager::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);

	_windowDims = Vec2D <int> (1024, 768);
	_window = SDL_CreateWindow("GraphicsTut", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowDims.x, _windowDims.y, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		ErrorManager::error("Window could not be created!", ErrorLevel::FATAL, true);
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		ErrorManager::error("SDL_GL context could not be created!", ErrorLevel::FATAL, true);
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		ErrorManager::error("Could not initialise glew!", ErrorLevel::FATAL, true);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_renderingManager = new RenderingManager(_window);
	if (_renderingManager == nullptr) {
		ErrorManager::error("Rendering Manager could not be created!", ErrorLevel::FATAL, true);
	}
	_renderingManager->initialiseRenderer();
}

void GameManager::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_time = SDL_GetTicks();
		if (!_inputManager->processInput()) {
			_gameState = GameState::EXIT;
		}

		if (!_renderingManager->render(_time)) {
			ErrorManager::error("Rendering failed!", ErrorLevel::FATAL, true);
		}
	}
}