#pragma once

#include <Xylem\Entity.h>

class Bullet : public Xylem::Entity
{
public:
    Bullet(glm::vec2 initialDirection, glm::vec2 initialPosition, float damage);
    ~Bullet();

    float getDamage() const { return _damage; }
private:
    float _damage;
};