#include "GameManager.h"

#include "ErrorManager.h"

#include <GL\glew.h>

#include <iostream>

GameManager::GameManager()
    : _window(nullptr), _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY)
{
}

GameManager::~GameManager()
{
}

void GameManager::run()
{
    initSystems();
    
    _sprite.init(-1, -1, 1, 1);

    gameLoop();
}

void GameManager::initSystems()
{
    // Initialise SDL.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Initiliase the SDL Window.
    _window = SDL_CreateWindow("Hemlock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (_window == nullptr) {
        ErrorManager::fatalError("SDL Window could not be created!");
    }

    // Initiliase the SDL GL Context.
    SDL_GLContext glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr) {
        ErrorManager::fatalError("SDL_GL context could not be created!");
    }

    // Initialise Glew.
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        ErrorManager::fatalError("Glew could not be initialised!");
    }

    // Set OpenGL to use double buffering.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Set the clear colour used by OpenGL.
    glClearColor(0, 1, 0, 1.0);

    initShaders();
}

void GameManager::initShaders()
{
    _colourProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
    _colourProgram.addAttribute("vertexPosition");
    _colourProgram.linkShaders();
}

void GameManager::gameLoop()
{
    while (_gameState != GameState::EXIT) {
        processInput();
        drawGame();
    }
}

void GameManager::processInput()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            _gameState = GameState::EXIT;
            break;
        case SDL_MOUSEMOTION:
            std::cout << "X - " << event.motion.x << " -- Y - " << event.motion.y << std::endl;
            break;
        }
    }
}

void GameManager::drawGame()
{
    // Clears to the specified depth.
    glClearDepth(1.0);
    // Cleas the colour and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _colourProgram.use();

    _sprite.draw();

    _colourProgram.unuse();

    SDL_GL_SwapWindow(_window);;
}
