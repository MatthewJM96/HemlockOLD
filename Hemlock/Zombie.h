#pragma once

#include "Civilian.h"
#include "Player.h"

#include <Xylem\Entity.h>

#include <vector>
#include <string>

class Zombie : public Xylem::Entity
{
public:
    Zombie(glm::vec2 initialDirection, glm::vec2 initialPosition, std::vector<std::string> validTargetEntityTypes = { "Civilian", "Player" });
    ~Zombie();

    bool update(const Player* player, const std::vector<Civilian*>& civilianEntities);
    void changeHealth(float amount);
private:
    std::vector<std::string> _validTargetEntityTypes;
};