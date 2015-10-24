#include "Bullet.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

Bullet::Bullet(float speed, glm::vec2 initialDirection, glm::vec2 initialPosition, int lifetime, float mass, float damage)
    : Entity(
        speed,
        initialDirection,
        initialPosition,
        glm::vec2(7.0f, 7.0f),
        lifetime,
        Xylem::ResourceManager::getTexture("Textures/Bullet.png"),
        Xylem::Colour{ 255,255,255,255 },
        mass,
        "Bullet"),
    _damage(damage)
{
}

Bullet::~Bullet()
{
}