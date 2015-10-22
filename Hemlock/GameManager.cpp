#include "GameManager.h"

#include <Xylem\Xylem.h>
#include <Xylem\ResourceManager.h>

#include <SDL\SDL.h>
#include <iostream>

GameManager::GameManager()
    : _screenWidth(1280), _screenHeight(720), _gameState(GameState::PLAY), _fps(60.0f), _maxFPS(90.0f), _player(nullptr)
{
    _camera.init(_screenWidth, _screenHeight);
}

GameManager::~GameManager() 
{
    // IMPLEMENT THIS!
}

void GameManager::run() 
{
    initSystems();

    gameLoop();
}

void GameManager::initSystems()
{
    Xylem::init();

    _fpsLimiter.init(_maxFPS);

    _window.create("Zombie Simulator 2015", _screenWidth, _screenHeight, 0);

    constructLevel(0);

    initShaders();

    _spriteBatch.init();
}

void GameManager::initShaders()
{
    // Compile our texture shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColour");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void GameManager::gameLoop()
{
    static int frameCount = 0;
    while (_gameState != GameState::EXIT) {
        _fpsLimiter.begin();

        processInput();

        _camera.update();

        _player->update(_inputManager, _bullets, _camera);
        std::cout << "X - " << _player->getPosition().x << " -- Y - " << _player->getPosition().y << std::endl;

        for (size_t i = 0; i < _bullets.size();) {
            if (_bullets[i]->update() == false) {
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
            } else {
                ++i;
            }
        }

        for (size_t i = 0; i < _civilians.size();) {
            if (_civilians[i]->update() == false) {
                _civilians[i] = _civilians.back();
                _civilians.pop_back();
            } else {
                ++i;
            }
        }

        for (size_t i = 0; i < _zombies.size();) {
            if (_zombies[i]->update(_player, _civilians) == false) {
                _zombies[i] = _zombies.back();
                _zombies.pop_back();
            } else {
                ++i;
            }
        }

        drawGame();

        _fps = _fpsLimiter.end();
        if (++frameCount % 600 == 0) {
            std::cout << _fps << std::endl;
        }
    }
}

void GameManager::processInput()
{
    const float CAMERA_SPEED = 1.2f;
    const float SCALE_SPEED = 0.1f;

    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

    if (_inputManager.isKeyPressed(SDLK_w)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_s)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -1.0f * CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_a)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(-1.0f * CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_d)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_q)) {
        _camera.setScale(_camera.getScale() + SCALE_SPEED);
    }
    if (_inputManager.isKeyPressed(SDLK_e)) {
        _camera.setScale(_camera.getScale() - SCALE_SPEED);
    }

    std::cout << "X - " << _camera.getPosition().x << " -- Y - " << _camera.getPosition().y << std::endl;
}

void GameManager::drawGame() 
{
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.use();

    glActiveTexture(GL_TEXTURE0);

    GLint textureLocation = _textureProgram.getUniformLocation("tex");
    glUniform1i(textureLocation, 0);

    GLuint pLocation = _textureProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = _camera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, false, &(cameraMatrix[0][0]));

    _spriteBatch.begin();

    _player->draw(_spriteBatch);

    for (size_t i = 0; i < _bullets.size(); ++i) {
        _bullets[i]->draw(_spriteBatch);
    }

    for (size_t i = 0; i < _civilians.size(); ++i) {
        _civilians[i]->draw(_spriteBatch);
    }

    for (size_t i = 0; i < _zombies.size(); ++i) {
        _zombies[i]->draw(_spriteBatch);
    }

    _spriteBatch.end();

    _spriteBatch.renderBatch();

    _textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void GameManager::constructLevel(unsigned int levelID)
{
    _player = new Player(glm::vec2(0.0f, 0.0f));
}
