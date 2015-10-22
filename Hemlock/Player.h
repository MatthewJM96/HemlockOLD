#pragma once

#include "Bullet.h"

#include <Xylem\Entity.h>
#include <Xylem\InputManager.h>
#include <Xylem\Camera2D.h>

#include <vector>

class Player : public Xylem::Entity
{
public:
    Player(glm::vec2 initialPosition);
    ~Player();

    bool update (const Xylem::InputManager& inputManager, std::vector<Bullet*>& bullets, const Xylem::Camera2D camera);
};

