#include "Zombie.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

Zombie::Zombie(glm::vec2 initialDirection, glm::vec2 initialPosition, std::vector<std::string> validTargetEntityTypes/* = { "Civilian", "Player" }*/)
    : Entity(
        1.0f,
        initialDirection,
        initialPosition,
        glm::vec2(5.0f, 5.0f),
        -1,
        Xylem::ResourceManager::getTexture("Textures/Circle.png"),
        Xylem::Colour{ 68,85,37,255 },
        1.0f,
        "Zombie"),
    _validTargetEntityTypes(validTargetEntityTypes)
{
}

Zombie::~Zombie()
{
}

bool Zombie::update(const Player* player, const std::vector<Civilian*>& civilianEntities)
{
    // Find nearest target.
    float minDist = glm::distance(player->getPosition(), _position);
    glm::vec2 nearestTargetPosition = player->getPosition();
    for (size_t i = 0; i < civilianEntities.size(); ++i) {
        // If valid target, check if closer than previous closest.
        if (std::find(_validTargetEntityTypes.begin(), _validTargetEntityTypes.end(), civilianEntities[i]->getEntityType()) != _validTargetEntityTypes.end()) {
            float dist = glm::distance(civilianEntities[i]->getPosition(), _position);
            if (dist < minDist) {
                minDist = dist;
                nearestTargetPosition = civilianEntities[i]->getPosition();
            }
        }
    }

    // Change direction to face target.
    _direction = glm::normalize(nearestTargetPosition - _position);

    // Update movement for motion towards target.
    _position += _direction * _maxSpeed;

    return true;
}
