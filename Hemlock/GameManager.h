#pragma once

#include "Bullet.h"
#include "Civilian.h"
#include "Zombie.h"
#include "Player.h"
#include "Level.h"

#include <Xylem/Window.h>
#include <Xylem/GLSLProgram.h>
#include <Xylem/Camera2D.h>
#include <Xylem/InputManager.h>
#include <Xylem/Timing.h>
#include <Xylem/SpriteBatch.h>

#include <vector>

enum class GameState {
    PLAY,
    EXIT_CONFIRMATION,
    EXIT
};

class GameManager
{
public:
    GameManager();
    ~GameManager();

    /// Runs the game
    void run();
private:

    /// Initializes the core systems
    void initSystems();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Updates various entities.
    void updatePlayer(float deltaTime);
    void updateBullets(float deltaTime);
    void updateCivilians(float deltaTime);
    void updateZombies(float deltaTime);

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Constructs the level
    void constructLevel(unsigned int levelID);

    /// Member Variables
    Xylem::Window _window; ///< The game window
    
    Xylem::GLSLProgram _textureProgram; ///< The shader program

    Xylem::InputManager _inputManager; ///< Handles input

    Xylem::Camera2D _camera; ///< Main Camera

    Xylem::FpsLimiter _fpsLimiter; ///< FPS Limiter

    Xylem::SpriteBatch _spriteBatch; ///< Sprite batch manager.

    Level _level;

    int _screenWidth; ///< Screen Width
    int _screenHeight; ///< Screen Height
    GameState _gameState; ///< Game State
    float _fps; ///< Current FPS
    float _maxFPS; ///< Target Maximum FPS
    int _levelID; ///< Current level ID.

    /// Entity Lists
    Player* _player; ///< Player Entity
    std::vector<Bullet*> _bullets; ///< Bullet Entities
    std::vector<Civilian*> _civilians; ///< Civilian Entities
    std::vector<Zombie*> _zombies; ///< Zombie Entities
};

