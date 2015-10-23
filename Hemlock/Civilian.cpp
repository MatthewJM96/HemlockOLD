#include "Civilian.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

#include <SDL\SDL.h>
#include <GLM\gtc\constants.hpp>
#include <GLM\gtx\vector_angle.hpp>

#include <random>

Civilian::Civilian(glm::vec2 initialDirection, glm::vec2 initialPosition)
    : Entity(
        1.2f,
        initialDirection,
        initialPosition,
        glm::vec2(25.0f, 25.0f),
        -1,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::Colour{ 255,174,66,255 },
        1.0f,
        "Civilian")
{
}

Civilian::~Civilian()
{
}

bool Civilian::update()
{
    // Generate a random angle.
    std::default_random_engine randomGenerator;
    randomGenerator.seed(SDL_GetTicks());
    std::uniform_real_distribution<float> distribution(0.0f, 2.0f * glm::pi<float>());
    float randomAngle = distribution(randomGenerator);

    // Calculate current direction as angle.
    float currentAngle = glm::angle(_position, glm::vec2(1.0f, 0.0f));

    // Calculate a weighted angle including the generated random angle.
    float weighting = 0.35f;
    float weightedAngle = (weighting * currentAngle + randomAngle) / (weighting + 1.0f);

    // Convert weighted angle back to a direction unit vector.
    _direction = glm::vec2(glm::cos(weightedAngle), glm::sin(weightedAngle));

    // Update movement for motion towards target.
    _position += _direction * _maxSpeed;

    return true;
}
