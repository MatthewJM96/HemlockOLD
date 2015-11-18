#include "Zombie.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

Zombie::Zombie(glm::vec2 initialDirection, glm::vec2 initialPosition, const Player* player, const std::vector<Civilian*>* civilianEntities, std::vector<std::string> validTargetEntityTypes/* = { "Civilian", "Player" }*/)
    : Entity(
        1.0f,
        initialDirection,
        initialPosition,
        glm::vec2(25.0f, 25.0f),
        100.0f,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::ColourRGBA8{ 68,85,37,255 },
        1.0f,
        "Zombie"),
    _validTargetEntityTypes(validTargetEntityTypes),
    _civilianEntities(civilianEntities),
    _player(player)
{
}

Zombie::~Zombie()
{
}

bool Zombie::update(float deltaTime)
{
    // Find nearest target.
    float minDist = glm::distance(_player->getPosition(), _position);
    glm::vec2 nearestTargetPosition = _player->getPosition();
    for (size_t i = 0; i < _civilianEntities->size(); ++i) {
        // If valid target, check if closer than previous closest.
        if (std::find(_validTargetEntityTypes.begin(), _validTargetEntityTypes.end(), (*_civilianEntities)[i]->getEntityType()) != _validTargetEntityTypes.end()) {
            float dist = glm::distance((*_civilianEntities)[i]->getPosition(), _position);
            if (dist < minDist) {
                minDist = dist;
                nearestTargetPosition = (*_civilianEntities)[i]->getPosition();
            }
        }
    }

    // Change direction to face target.
    _direction = glm::normalize(nearestTargetPosition - _position);

    // Update movement for motion towards target.
    _position += _direction * _maxSpeed * deltaTime;

    if (_lifetime <= 0) {
        return false;
    }
    return true;
}

void Zombie::changeHealth(float amount)
{
    _lifetime += amount;
}
