#include "Player.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

#include <SDL\SDL.h>
#include <GLM\glm.hpp>

Player::Player(glm::vec2 initialPosition)
    : Entity(
        1.2f,
        glm::vec2(0.0f, 0.0f),
        initialPosition,
        glm::vec2(25.0f, 25.0f),
        -1,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::Colour{ 16,190,239,255 },
        "Player")
{
}

Player::~Player()
{
}

bool Player::update(const Xylem::InputManager & inputManager, std::vector<Bullet*>& bullets, const Xylem::Camera2D camera)
{
    if (inputManager.isKeyPressed(SDLK_w)) {
        _position += glm::vec2(0.0f, 1.0f) * _maxSpeed;
    }
    if (inputManager.isKeyPressed(SDLK_s)) {
        _position += glm::vec2(0.0f, -1.0f) * _maxSpeed;
    }
    if (inputManager.isKeyPressed(SDLK_a)) {
        _position += glm::vec2(-1.0f, 0.0f) * _maxSpeed;
    }
    if (inputManager.isKeyPressed(SDLK_d)) {
        _position += glm::vec2(1.0f, 0.0f) * _maxSpeed;
    }
    if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
        glm::vec2 mouseCoords = inputManager.getMouseCoords();
        mouseCoords = camera.convertScreenToWorld(mouseCoords);

        glm::vec2 direction = glm::normalize(mouseCoords - (_position + (_size / 2.0f)));

        bullets.push_back(new Bullet(direction, _position + (_size / 2.0f)));
    }

    return true;
}
