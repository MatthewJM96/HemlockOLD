#include "Level.h"

#include <Xylem\IOManager.h>
#include <Xylem\ResourceManager.h>

#include <iostream>

Level::Level()
{
}

Level::~Level()
{
}

void Level::init()
{
    _tileTypes.emplace_back('R', Xylem::ResourceManager::getTexture("Textures/stonebricks_granite.png"));
    _tileTypes.emplace_back('G', Xylem::ResourceManager::getTexture("Textures/glass.png"));
    _tileTypes.emplace_back('B', Xylem::ResourceManager::getTexture("Textures/bricks.png"));
    _tileTypes.emplace_back('L', Xylem::ResourceManager::getTexture("Textures/bricks_light.png"));
    _tileTypes.emplace_back('.', Xylem::ResourceManager::getTexture("Textures/wooden_planks_coniferous_side.png"));
}

void Level::clean()
{
    _tiles.clear();
    _gunData.clear();
    _startingHumans = 0;
    _mapWidth = 0;
}

void Level::load(int levelID)
{
    std::string filePath = "Levels/level_" + std::to_string(levelID) + ".txt";
    std::vector<std::string> levelBuffer;
    Xylem::IOManager::readFileLinesToBuffer(filePath, levelBuffer);

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
                _tiles.push_back(lineC[i]);
            }
        }
    }
}

void Level::draw(Xylem::SpriteBatch & _spriteBatch, const Xylem::Camera2D& camera)
{

    int i = 0;
    for (char tile : _tiles) {
        if (tile == 'Z' || tile == '@') {
            tile = '.';
        }
        for (Tile tileType : _tileTypes) {
            if (tileType.getTileID() == tile) {
                int x = (i % _mapWidth) * 32.0f;
                int y = std::floor(i / _mapWidth) * 32.0f;
                if (!camera.isOnScreen(glm::vec2(x, y), glm::vec2(32.0f, 32.0f))) {
                    break;
                }
                glm::vec4 uvRectangle(0.0f, 0.0f, 1.0f, 1.0f);
                glm::vec4 destinationRectangle(x, y, 32.0f, 32.0f);
                _spriteBatch.draw(destinationRectangle, uvRectangle, tileType.getTextureID(), 0.0f, tileType.getColour());
            }
        }
        ++i;
    }
}

void Level::collideEntity(Xylem::Entity & entity)
{
    //TODO
}

bool Level::tileAtLocation(glm::vec2 location)
{
    return false;
}
