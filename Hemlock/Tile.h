#pragma once

#include <Xylem\Entity.h>
#include <Xylem\Vertex.h>

#include <GL\glew.h>

class Tile
{
public:
    Tile(char id, Xylem::GLTexture glTexture, Xylem::Colour colour = { 255, 255, 255, 255 });
    ~Tile();

    char getTileID() const { return _id; }
    GLuint getTextureID() const { return _texture.id; }
    Xylem::Colour getColour() const { return _colour; }
private:
    char _id;
    Xylem::GLTexture _texture;
    Xylem::Colour _colour;
};

