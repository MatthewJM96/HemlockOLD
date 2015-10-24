#pragma once

#include "Tile.h"

#include <Xylem\SpriteBatch.h>
#include <Xylem\Camera2D.h>

#include <GLM\glm.hpp>

class Level
{
public:
    Level();
    ~Level();

    void init();
    void clean();
    void load(int levelID);
    void draw(Xylem::SpriteBatch& _spriteBatch, const Xylem::Camera2D& camera);

    void collideEntity(Xylem::Entity& entity);

    int getStartingHumanCount() const { return _startingHumans; }
    std::vector<std::vector<float>> getGunData() const { return _gunData; }

    bool tileAtLocation(glm::vec2 location);
private:
    std::vector<char> _tiles;
    std::vector<Tile> _tileTypes;
    int _startingHumans;
    std::vector<std::vector<float>> _gunData;
    int _mapWidth;
};

