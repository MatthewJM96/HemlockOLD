#pragma once

#include <Xylem\Entity.h>

class Bullet : public Xylem::Entity
{
public:
    Bullet(float speed, glm::vec2 initialDirection, glm::vec2 initialPosition, int lifetime, float mass, float damage);
    ~Bullet();

    float getDamage() const { return _damage; }
private:
    float _damage;
};