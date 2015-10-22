#pragma once

#include <Xylem\Entity.h>

class Bullet : public Xylem::Entity
{
public:
    Bullet(glm::vec2 initialDirection, glm::vec2 initialPosition);
    ~Bullet();
};