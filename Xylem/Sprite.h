#pragma once

#include "GLTexture.h"

#include <GL/glew.h>

#include <string>

namespace Xylem
{
    class Sprite
    {
    public:
        Sprite();
        ~Sprite();

        void init(float x, float y, float width, float height, std::string texturePath);

        void draw();
    private:
        int _x;
        int _y;
        int _width;
        int _height;
        GLuint _vboID;
        GLTexture _texture;
    };
}