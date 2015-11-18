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
    Player(glm::vec2 initialPosition, const Xylem::InputManager* inputManager, const Xylem::Camera2D* camera);
    ~Player();

    void giveGun(Gun* gun) { _guns.push_back(gun); }

    bool update (float deltaTime) override;
private:
    int _currentGunIndex;
    std::vector<Gun*> _guns;

    const Xylem::Camera2D* _camera;
    const Xylem::InputManager* _inputManager;
};

