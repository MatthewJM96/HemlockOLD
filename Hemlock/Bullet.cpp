#include "Bullet.h"

#include <Xylem\ResourceManager.h>
#include <Xylem\Vertex.h>

Bullet::Bullet(glm::vec2 initialDirection, glm::vec2 initialPosition)
    : Entity(
        5.0f, 
        initialDirection, 
        initialPosition,
        glm::vec2(8.0f, 8.0f), 
        600, 
        Xylem::ResourceManager::getTexture("Textures/Bullet.png"), 
        Xylem::Colour{ 255,255,255,255 },
        "Bullet")
{
}

Bullet::~Bullet()
{
}