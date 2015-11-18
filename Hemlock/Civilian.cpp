#include "Civilian.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

#include <SDL\SDL.h>
#include <GLM\gtc\constants.hpp>
#include <GLM\gtx\vector_angle.hpp>

#include <random>
#include <iostream>

Civilian::Civilian(glm::vec2 initialDirection, glm::vec2 initialPosition)
    : Entity(
        1.2f,
        initialDirection,
        initialPosition,
        glm::vec2(25.0f, 25.0f),
        100.0f,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::ColourRGBA8{ 255,174,66,255 },
        1.0f,
        "Civilian"),
    _updateCount(0),
    _nextDirUpdate(0)
{
}

Civilian::~Civilian()
{
}

bool Civilian::update(float deltaTime)
{
    // If update count is equal to next direction update count, update civilian direction.
    if (_updateCount == _nextDirUpdate) {
        // Generate a random angle.
        std::default_random_engine randomGenerator;
        randomGenerator.seed(SDL_GetTicks() * _position.x);
        std::uniform_real_distribution<float> angDistribution(0.0f, 2.0f * glm::pi<float>());
        float randomAngle = angDistribution(randomGenerator);

        // Generate random delay till next dir update.
        std::uniform_int_distribution<int> nextUpdateDistribution(10, 120);
        _nextDirUpdate = nextUpdateDistribution(randomGenerator);

        _direction = glm::vec2(glm::cos(randomAngle), glm::sin(randomAngle));
        _updateCount = 0;
    }

    // Update movement for motion towards target.
    _position += _direction * _maxSpeed * deltaTime;

    ++_updateCount;
    if (_lifetime <= 0) {
        return false;
    }
    return true;
}

void Civilian::changeHealth(float amount)
{
    _lifetime += amount;
}
