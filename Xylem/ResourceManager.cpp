#include "ResourceManager.h"

#include <iostream>

namespace Xylem
{
    TextureCache ResourceManager::_textureCache;

    /// Gets a texture from the given texture path.
    GLTexture ResourceManager::getTexture(std::string texturePath)
    {
        return _textureCache.getTexture(texturePath);
    }
}