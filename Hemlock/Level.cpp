#include "Level.h"

#include <Xylem\IOManager.h>
#include <Xylem\ResourceManager.h>

#include <algorithm>
#include <iostream>

Level::Level()
{
}

Level::~Level()
{
}

/// Initialise a level.
void Level::init()
{
    _tileTypes.emplace_back('R', Xylem::ResourceManager::getTexture("Textures/stonebricks_granite.png"));
    _tileTypes.emplace_back('G', Xylem::ResourceManager::getTexture("Textures/glass.png"));
    _tileTypes.emplace_back('B', Xylem::ResourceManager::getTexture("Textures/bricks.png"));
    _tileTypes.emplace_back('L', Xylem::ResourceManager::getTexture("Textures/bricks_light.png"));
    _tileTypes.emplace_back('.', Xylem::ResourceManager::getTexture("Textures/wooden_planks_coniferous_side.png"));
}

/// Clean the level out ready for loading a new one.
void Level::clean()
{
    _tiles.clear();
    _gunData.clear();
    _startingHumans = 0;
    _mapWidth = 0;
}

/// Load a level by level ID.
void Level::load(int levelID)
{
    std::string filePath = "Levels/level_" + std::to_string(levelID) + ".txt";
    std::vector<std::string> levelBuffer;
    Xylem::IOManager::readFileLinesToBuffer(filePath, levelBuffer);

    int j = 0;
    for (std::string line : levelBuffer) {
        std::string token = line.substr(0, line.find(" "));
        if (token == "Humans:") {
            _startingHumans = atoi(line.substr(line.find(" ") + 1, line.size()).c_str());
        } else if (token == "Weapon:") {
            std::string leftover = line.substr(line.find(" ") + 1, line.size());
            std::vector<float> _gunDataEntry;
            while (leftover.find(",")) {
                // If no commas remain, deal with as special case.
                if (leftover.find(",") == std::string::npos) {
                    _gunDataEntry.push_back(atof(token.c_str()));
                    break;
                }
                token = leftover.substr(0, leftover.find(","));
                _gunDataEntry.push_back(atof(token.c_str()));
                leftover = leftover.substr(leftover.find(",") + 1, leftover.size());
            }
        } else {
            _mapWidth = line.size();
            const char* lineC = line.c_str();
            for (int i = 0; i < line.size(); ++i) {
                if (lineC[i] == '@') {
                    _playerStartPosition = glm::vec2(i * TILE_WIDTH + TILE_WIDTH / 2.0f, j * _mapWidth + TILE_WIDTH / 2.0f);
                } else if (lineC[i] == 'Z') {
                    _zombieStartPositions.emplace_back(i * TILE_WIDTH + TILE_WIDTH / 2.0f, j * _mapWidth + TILE_WIDTH / 2.0f);
                }
                _tiles.push_back(lineC[i]);
            }
            ++j;
        }
    }
}

/// Draw a level to the sprite batch.
void Level::draw(Xylem::SpriteBatch & _spriteBatch, const Xylem::Camera2D& camera)
{

    int i = 0;
    for (char tile : _tiles) {
        if (tile == 'Z' || tile == '@') {
            tile = '.';
        }
        for (Tile tileType : _tileTypes) {
            if (tileType.getTileID() == tile) {
                int x = (i % _mapWidth) * TILE_WIDTH;
                int y = std::floor(i / _mapWidth) * TILE_WIDTH;
                if (!camera.isOnScreen(glm::vec2(x, y), glm::vec2(TILE_WIDTH, TILE_WIDTH))) {
                    break;
                }
                glm::vec4 uvRectangle(0.0f, 0.0f, 1.0f, 1.0f);
                glm::vec4 destinationRectangle(x, y, TILE_WIDTH, TILE_WIDTH);
                _spriteBatch.draw(destinationRectangle, uvRectangle, tileType.getTextureID(), 0.0f, tileType.getColour());
            }
        }
        ++i;
    }
}

/// Collide an entity with the level.
void Level::collideEntity(Xylem::Entity & entity)
{
    // Stores the centre position of each collidable tile.
    std::vector<glm::vec2> collidableTilePositions;

    // Check each corner of the entity for if the tile the corner exists on is collidable.
    // NOTE: This collision method assumes the entities will always be smaller than or equal to the size of one tile.
#define GET_TILE_POS(a) std::floor((a) / TILE_WIDTH) * TILE_WIDTH + TILE_WIDTH / 2.0f
    // Bottom Left.
    if (collidableTileAtLocation(entity.getPosition())) {
        collidableTilePositions.emplace_back(GET_TILE_POS(entity.getPosition().x),
                                             GET_TILE_POS(entity.getPosition().y));
    }
    // Bottom Right.
    if (collidableTileAtLocation(entity.getPosition() + glm::vec2(entity.getSize().x, 0.0f))) {
        collidableTilePositions.emplace_back(GET_TILE_POS(entity.getPosition().x + entity.getSize().x),
                                             GET_TILE_POS(entity.getPosition().y));
    }
    // Top Left.
    if (collidableTileAtLocation(entity.getPosition() + glm::vec2(0.0f, entity.getSize().y))) {
        collidableTilePositions.emplace_back(GET_TILE_POS(entity.getPosition().x),
                                             GET_TILE_POS(entity.getPosition().y + entity.getSize().y));
    }
    // Top Right.
    if (collidableTileAtLocation(entity.getPosition() + entity.getSize())) {
        collidableTilePositions.emplace_back(GET_TILE_POS(entity.getPosition().x + entity.getSize().x),
                                             GET_TILE_POS(entity.getPosition().y + entity.getSize().y));
    }

    for (auto tilePosition : collidableTilePositions) {
        float distanceToCollideX = (entity.getSize().x + TILE_WIDTH) / 2.0f;
        float distanceToCollideY = (entity.getSize().y + TILE_WIDTH) / 2.0f;

        glm::vec2 entityCentrePosition = entity.getPosition() + (entity.getSize() / 2.0f);

        glm::vec2 rVect = entityCentrePosition - tilePosition;

        float depthX = distanceToCollideX - abs(rVect.x);
        float depthY = distanceToCollideY - abs(rVect.y);

        if (depthX > 0.0f && depthY > 0.0f) {
            if (std::max(depthX, 0.0f) < std::max(depthY, 0.0f)) {
                if (rVect.x < 0) {
                    entity.setPosition(entity.getPosition() - glm::vec2(depthX, 0.0f));
                } else {
                    entity.setPosition(entity.getPosition() + glm::vec2(depthX, 0.0f));
                }
            } else {
                if (rVect.y < 0) {
                    entity.setPosition(entity.getPosition() - glm::vec2(0.0f, depthY));
                } else {
                    entity.setPosition(entity.getPosition() + glm::vec2(0.0f, depthY));
                }
            }
        }
    }
}

/// Determine if the tile at the given location is collidable.
bool Level::collidableTileAtLocation(glm::vec2 location)
{
    // Calculate the index of the tile at the given location.
    int tileIndex = (int)(std::floor(location.x / TILE_WIDTH) + std::floor(location.y / TILE_WIDTH) * _mapWidth);

    // If out of bounds of the level return false.
    if (tileIndex < 0 || tileIndex > _tiles.size()) {
        return false;
    }

    // Get the tile ID and check if it is collidable.
    char tileId = _tiles[tileIndex];
    if (tileId == 'R' ||
        tileId == 'G' ||
        tileId == 'B' ||
        tileId == 'L') {
        return true;
    }
    return false;
}