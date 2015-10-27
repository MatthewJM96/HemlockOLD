#include "TextureCache.h"

#include "ImageLoader.h"

namespace Xylem
{
    TextureCache::TextureCache()
    {
    }

    TextureCache::~TextureCache()
    {
    }

    /// Gets a texture with given texture path from cache if already loaded, otherwise loads it.
    GLTexture TextureCache::getTexture(std::string texturePath)
    {
        auto it = _textureMap.find(texturePath);

        if (it == _textureMap.end()) {
            GLTexture newTexture = ImageLoader::loadPNG(texturePath);

            _textureMap.insert(make_pair(texturePath, newTexture));

            return newTexture;
        }

        return it->second;
    }
}