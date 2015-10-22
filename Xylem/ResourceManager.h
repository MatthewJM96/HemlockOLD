#pragma once

#include "TextureCache.h"

namespace Xylem
{
    class ResourceManager
    {
    public:

        static GLTexture getTexture(std::string texturePath);
    private:
        ResourceManager();

        static TextureCache _textureCache;
    };
}