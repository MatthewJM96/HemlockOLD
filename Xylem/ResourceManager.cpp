#include "ResourceManager.h"

#include <iostream>

namespace Xylem
{
    TextureCache ResourceManager::_textureCache;

    ResourceManager::ResourceManager() {}

    GLTexture ResourceManager::getTexture(std::string texturePath)
    {
        return _textureCache.getTexture(texturePath);
    }
}