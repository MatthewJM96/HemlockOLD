#include "Tile.h"

Tile::Tile(char id, Xylem::GLTexture glTexture, Xylem::Colour colour)
    : _id(id), _texture(glTexture), _colour(colour)
{
}

Tile::~Tile()
{
}
