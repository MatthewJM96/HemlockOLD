#include "TextureCache.h"

#include "ImageLoader.h"

#include <iostream>

namespace Xylem
{
    TextureCache::TextureCache()
    {
    }

    TextureCache::~TextureCache()
    {
    }

    GLTexture TextureCache::getTexture(std::string texturePath)
    {
        // auto == std::map<std::string, GLTexture>::iterator
        auto it = _textureMap.find(texturePath);

        if (it == _textureMap.end()) {
            GLTexture newTexture = ImageLoader::loadPNG(texturePath);

            _textureMap.insert(make_pair(texturePath, newTexture));

            //std::cout << "Loaded cached texture.\n" << std::endl;
            return newTexture;
        }

        //std::cout << "Loaded texture.\n" << std::endl;
        return it->second;
    }
}