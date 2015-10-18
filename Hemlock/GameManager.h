#pragma once

#include "Sprite.h"
#include "GLSLProgram.h"

#include <SDL/SDL.h>

enum class GameState {
    PLAY,
    EXIT
};

class GameManager
{
public:
    GameManager();
    ~GameManager();

    void run();
private:
    SDL_Window* _window;
    int _screenWidth;
    int _screenHeight;
    GameState _gameState;

    void initSystems();
    void initShaders();
    void gameLoop();
    void processInput();
    void drawGame();

    Sprite _sprite;

    GLSLProgram _colourProgram;
};

