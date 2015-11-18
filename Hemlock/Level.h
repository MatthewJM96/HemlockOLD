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
    glm::vec2 getPlayerStartingLocation() const { return _playerStartPosition; }
    std::vector<glm::vec2> getZombieStartPositions() const { return _zombieStartPositions; }

    int getMapWidth() const { return _mapWidth; }
    int getMapHeight() const { return (int)std::ceil(_tiles.size() / _mapWidth); }
private:
    const float TILE_WIDTH = 32.0f;

    std::vector<char> _tiles;
    std::vector<Tile> _tileTypes;
    int _startingHumans;
    std::vector<std::vector<float>> _gunData;
    glm::vec2 _playerStartPosition;
    std::vector<glm::vec2> _zombieStartPositions;
    int _mapWidth;

    bool collidableTileAtLocation(glm::vec2 location);
};

