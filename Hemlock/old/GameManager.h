#pragma once

#include <SDL\SDL.h>
#include <Xylem\Window.h>
#include <Xylem\Sprite.h>
#include <Xylem\GLSLProgram.h>
#include <Xylem\GLTexture.h>
#include <Xylem\Camera2D.h>
#include <Xylem\SpriteBatch.h>
#include <Xylem\InputManager.h>
#include <Xylem\Timing.h>
#include <Xylem\Bullet.h>

#include <vector>

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
    Xylem::Window _window;
    int _screenWidth;
    int _screenHeight;
    GameState _gameState;
    float _fps;
    float _maxFPS;

    void initSystems();
    void initShaders();
    void gameLoop();
    void processInput();
    void drawGame();

    Xylem::InputManager _inputManager;
    Xylem::FpsLimiter _fpsLimiter;

    std::vector<Xylem::Bullet> _bullets;

    Xylem::SpriteBatch _spriteBatch;

    Xylem::GLSLProgram _colourProgram;
    Xylem::Camera2D _camera;
};

