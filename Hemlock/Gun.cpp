#include "Gun.h"

#include <SDL/SDL.h>
#include <GLM/gtc/constants.hpp>

#include <random>

Gun::Gun(int fireRate, float speed, float mass, float damage, int range, int bulletCountPerShot, float fireSpray, std::vector<Bullet*>* bullets)
    : _fireRate(fireRate), _speed(speed), _mass(mass), _damage(damage), _range(range), _bulletCountPerShot(bulletCountPerShot), _fireSpray(fireSpray), _bullets(bullets), _lastFireTime(0)
{
}

Gun::~Gun()
{
}

void Gun::fire(glm::vec2 position, glm::vec2 direction)
{
    //std::cout << SDL_GetTicks() - _lastFireTime << std::endl;

    // Check if may fire.
    if ((SDL_GetTicks() - _lastFireTime) < (1000 / _fireRate)) {
        return;
    }
    
    // Prepare some angles.
    float minAngle = glm::atan(direction.y / direction.x) - (_fireSpray / 2.0f);
    float anglePerBullet = _fireSpray / _bulletCountPerShot;

    // Calculate lifetime.
    int lifetime = (int)(_range * PX_PER_UNIT_RANGE / _speed);

    // Create a normalised float generator.
    std::default_random_engine randomGenerator;
    randomGenerator.seed(SDL_GetTicks());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    for (size_t i = 0; i < _bulletCountPerShot; ++i) {
        // Generate normliased float.
        float normalisedFloat = distribution(randomGenerator);

        // Get bullet angle.
        float bulletAngle = minAngle + anglePerBullet * normalisedFloat + anglePerBullet * i;

        // Calculate cartesian direction of projectile.
        glm::vec2 bulletDirection = glm::vec2(glm::cos(bulletAngle) , glm::sin(bulletAngle));
        if ((direction.x / abs(direction.x)) == -1) {
            bulletDirection *= -1.0f; // Quick fix, probably need to reassess camera maths. Else could be even/odd nature of sine functions.
        }

        _bullets->push_back(new Bullet(_speed, bulletDirection, position, lifetime, _mass, _damage));
    }

    _lastFireTime = SDL_GetTicks();
}