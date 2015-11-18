#pragma once

#include <Xylem\Entity.h>
#include <Xylem\Vertex.h>

#include <GL\glew.h>

class Tile
{
public:
    Tile(char id, Xylem::GLTexture glTexture, Xylem::ColourRGBA8 colour = { 255, 255, 255, 255 });
    ~Tile();

    char getTileID() const { return _id; }
    GLuint getTextureID() const { return _texture.id; }
    Xylem::ColourRGBA8 getColour() const { return _colour; }
private:
    char _id;
    Xylem::GLTexture _texture;
    Xylem::ColourRGBA8 _colour;
};

