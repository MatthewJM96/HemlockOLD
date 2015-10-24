#pragma once

#include "Bullet.h"

#include <vector>

class Gun
{
public:
    Gun(int fireRate, float speed, float mass, float damage, int range, int bulletCountPerShot, float fireSpray, std::vector<Bullet*>* bullets);
    ~Gun();

    void fire(glm::vec2 position, glm::vec2 direction);
private:
    int _fireRate; // Bullets per 1000 ticks.
    float _speed;
    float _mass;
    float _damage;
    int _range;
    int _bulletCountPerShot;
    float _fireSpray;
    std::vector<Bullet*>* _bullets;

    unsigned int _lastFireTime;

    const int PX_PER_UNIT_RANGE = 32;
};

