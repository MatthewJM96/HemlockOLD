#pragma once

#include "Civilian.h"
#include "Player.h"

#include <Xylem\Entity.h>

#include <vector>
#include <string>

class Zombie : public Xylem::Entity
{
public:
    Zombie(glm::vec2 initialDirection, glm::vec2 initialPosition, const Player* player, const std::vector<Civilian*>* civilianEntities, std::vector<std::string> validTargetEntityTypes = { "Civilian", "Player" });
    ~Zombie();

    bool update(float DeltaTime) override;
    void changeHealth(float amount);
private:
    std::vector<std::string> _validTargetEntityTypes;
    const std::vector<Civilian*>* _civilianEntities;
    const Player* _player;
};