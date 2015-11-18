#pragma once

#include <Xylem\Entity.h>

class Civilian : public Xylem::Entity
{
public:
    Civilian(glm::vec2 initialDirection, glm::vec2 initialPosition);
    ~Civilian();

    bool update(float deltaTime) override;
    void changeHealth(float amount);
private:
    int _updateCount;
    int _nextDirUpdate;
};

