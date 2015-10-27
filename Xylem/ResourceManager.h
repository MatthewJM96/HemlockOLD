#pragma once

#include "TextureCache.h"

namespace Xylem
{
    class ResourceManager
    {
    public:
        static GLTexture getTexture(std::string texturePath);
    private:
        static TextureCache _textureCache;
    };
}