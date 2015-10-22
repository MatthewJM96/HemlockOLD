#pragma once

#include <GL/glew.h>

namespace Xylem
{
    struct Dimension {
        GLuint width;
        GLuint height;
    };

    struct GLTexture
    {
        GLuint id;
        Dimension dimension;
    };
}