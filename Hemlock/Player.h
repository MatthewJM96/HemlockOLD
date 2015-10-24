#pragma once

#include "Bullet.h"
#include "Gun.h"

#include <Xylem\Entity.h>
#include <Xylem\InputManager.h>
#include <Xylem\Camera2D.h>

#include <vector>

class Player : public Xylem::Entity
{
public:
    Player(glm::vec2 initialPosition);
    ~Player();

    void giveGun(Gun* gun) { _guns.push_back(gun); _currentGun = gun; }

    bool update (const Xylem::InputManager& inputManager, std::vector<Bullet*>& bullets, const Xylem::Camera2D camera);
private:
    Gun* _currentGun;
    std::vector<Gun*> _guns;
};

