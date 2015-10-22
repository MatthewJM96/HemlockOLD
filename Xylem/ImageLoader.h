#pragma once

#include "GLTexture.h"

#include "IOManager.h"
#include "ErrorManager.h"

#include <string>

namespace Xylem
{
    class ImageLoader
    {
    public:
        static GLTexture loadPNG(std::string filePath);
    };
}