#include "Entity.h"

#include <iostream>

namespace Xylem
{
    GLTexture Entity::_texture;

    Entity::Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, Colour colour, std::string entityType)
        : _maxSpeed(maxSpeed), _direction(initialDirection), _position(initialPosition), _size(size), _lifetime(lifetime), _colour(colour), _entityType(entityType)
    {
        Entity::_texture = texture;
    }

    Entity::~Entity()
    {
    }

    void Entity::draw(SpriteBatch & _spriteBatch)
    {
        glm::vec4 uvRectangle(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 destinationRectangle(_position.x, _position.y, _size.x, _size.y);

        /*std::cout << "DestRect --- " << destinationRectangle.x << " - " << destinationRectangle.y << " - " << destinationRectangle.z << " - " << destinationRectangle.w << std::endl;
        std::cout << "uvRectangle --- " << uvRectangle.x << " - " << uvRectangle.y << " - " << uvRectangle.z << " - " << uvRectangle.w << std::endl;
        std::cout << "Texture ID --- " << _texture.id << std::endl;
        std::cout << "Colour --- " << (int)_colour.r << " - " << (int)_colour.g << " - " << (int)_colour.b << " - " << (int)_colour.a << std::endl;
*/
        _spriteBatch.draw(destinationRectangle, uvRectangle, _texture.id, 0.0f, _colour);
    }

    bool Entity::update()
    {
        _position += _direction * _maxSpeed;

        std::cout << _lifetime << std::endl;
        if (_lifetime > 0 && --_lifetime == 0) {
            return false;
        }
        return true;
    }
}