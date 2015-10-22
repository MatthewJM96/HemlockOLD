#include "GameManager.h"

#include <GL\glew.h>
#include <Xylem\Xylem.h>
#include <Xylem\ErrorManager.h>
#include <Xylem\ImageLoader.h>
#include <Xylem\ResourceManager.h>

#include <iostream>

GameManager::GameManager()
    : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _fps(60.0f), _maxFPS(90.0f)
{
    _camera.init(_screenWidth, _screenHeight);
}

GameManager::~GameManager()
{
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

    _window.create("Hemlock", _screenWidth, _screenHeight, 0);

    initShaders();

    _spriteBatch.init();
}

void GameManager::initShaders()
{
    _colourProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
    _colourProgram.addAttribute("vertexPosition");
    _colourProgram.addAttribute("vertexColour");
    _colourProgram.addAttribute("vertexUV");
    _colourProgram.linkShaders();
}

void GameManager::gameLoop()
{
    static int frameCount = 0;
    while (_gameState != GameState::EXIT) {
        _fpsLimiter.begin();

        processInput();


        _camera.update();

        for (size_t i = 0; i < _bullets.size();) {
            if (_bullets[i].update() == false) {
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
            } else {
                ++i;
            }
        }

        drawGame();
        
        _fps = _fpsLimiter.end();

        ++frameCount;
        if (frameCount % 1000 == 0) {
            std::cout << _fps << std::endl;
        }
    }
}

void GameManager::processInput()
{
    SDL_Event evnt;
    
    const float CAMERA_SPEED = 5.0f;
    const float SCALE_SPEED = 0.1f;

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
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -1.0f * CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_s)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_a)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_d)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(-1.0f * CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_q)) {
        _camera.setScale(_camera.getScale() + SCALE_SPEED);
    }
    if (_inputManager.isKeyPressed(SDLK_e)) {
        _camera.setScale(_camera.getScale() - SCALE_SPEED);
    }
    if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
        glm::vec2 mouseCoords = _inputManager.getMouseCoords();
        mouseCoords = _camera.convertScreenToWorld(mouseCoords);

        glm::vec2 playerPosition(0.0f);
        glm::vec2 direction = mouseCoords - playerPosition;
        direction = glm::normalize(direction);

        _bullets.emplace_back(playerPosition, direction, 5.0f, 600);
    }
}

void GameManager::drawGame()
{
    // Clears to the specified depth.
    glClearDepth(1.0);
    // Cleas the colour and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _colourProgram.use();

    glActiveTexture(GL_TEXTURE0);

    GLint textureLocation = _colourProgram.getUniformLocation("tex");
    glUniform1i(textureLocation, 0);

    /*GLuint timeLocation = _colourProgram.getUniformLocation("time");
    glUniform1f(timeLocation, _frameTime);*/

    GLuint pLocation = _colourProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = _camera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, false, &(cameraMatrix[0][0]));

    _spriteBatch.begin();

    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Xylem::GLTexture texture = Xylem::ResourceManager::getTexture("Textures/CharacterLeft_Standing.png");
    Xylem::Colour colour;
    colour.a = 255;
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;
    _spriteBatch.draw(pos, uv, texture.id, 0.0f, colour);

    for (size_t i = 0; i < _bullets.size(); ++i) {
        _bullets[i].draw(_spriteBatch);
    }

    /*for (size_t i = 0; i < 5000; ++i) {
        _spriteBatch.draw(pos, uv, texture.id, 0.0f, colour);
    }*/

    _spriteBatch.end();

    _spriteBatch.renderBatch();

    _colourProgram.unuse();

    _window.swapBuffer();
}
