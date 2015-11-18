#include "Player.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

#include <SDL\SDL.h>
#include <GLM\glm.hpp>

#include <iostream>

Player::Player(glm::vec2 initialPosition, const Xylem::InputManager* inputManager, const Xylem::Camera2D* camera)
    : Entity(
        1.3f,
        glm::vec2(0.0f, 0.0f),
        initialPosition,
        glm::vec2(25.0f, 25.0f),
        -1,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::ColourRGBA8{ 16,190,239,255 },
        1.0f,
        "Player"),
    _inputManager(inputManager),
    _camera(camera),
    _currentGunIndex(0)
{
}

Player::~Player()
{
}

bool Player::update(float deltaTime)
{
    if (_inputManager->isKeyDown(SDLK_w)) {
        _position += glm::vec2(0.0f, 1.0f) * _maxSpeed * deltaTime;
    }
    if (_inputManager->isKeyDown(SDLK_s)) {
        _position += glm::vec2(0.0f, -1.0f) * _maxSpeed * deltaTime;
    }
    if (_inputManager->isKeyDown(SDLK_a)) {
        _position += glm::vec2(-1.0f, 0.0f) * _maxSpeed * deltaTime;
    }
    if (_inputManager->isKeyDown(SDLK_d)) {
        _position += glm::vec2(1.0f, 0.0f) * _maxSpeed * deltaTime;
    }

    if (_inputManager->isKeyDown(SDL_BUTTON_LEFT)) {
        glm::vec2 mouseCoords = _inputManager->getMouseCoords();
        mouseCoords = _camera->convertScreenToWorld(mouseCoords);
        
        glm::vec2 direction = glm::normalize(mouseCoords - (_position + (_size / 2.0f)));
        
        _guns[_currentGunIndex]->fire(_position, direction);
    }

    if (_inputManager->isKeyPressed(SDLK_1)) {
        _currentGunIndex = 0;
    }
    if (_inputManager->isKeyPressed(SDLK_2)) {
        _currentGunIndex = 1;
    }
    if (_inputManager->isKeyPressed(SDLK_3)) {
        _currentGunIndex = 2;
    }
    if (_inputManager->isKeyPressed(SDLK_4)) {
        _currentGunIndex = 3;
    }

    if (_inputManager->isKeyPressed(SDLK_LEFT)) {
        --_currentGunIndex;
    }

    if (_inputManager->isKeyPressed(SDLK_RIGHT)) {
        ++_currentGunIndex;
    }

    return true;
}
