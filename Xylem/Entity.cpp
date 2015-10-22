#include "Entity.h"

namespace Xylem
{
    Entity::Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, Colour colour, std::string entityType)
        : _maxSpeed(maxSpeed), _direction(initialDirection), _position(initialPosition), _size(size), _lifetime(lifetime), _texture(texture), _colour(colour), _entityType(entityType)
    {
    }

    Entity::~Entity()
    {
    }

    void Entity::draw(SpriteBatch & _spriteBatch)
    {
        glm::vec4 uvRectangle(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 destinationRectangle(_position.x, _position.y, _size.x, _size.y);

        _spriteBatch.draw(destinationRectangle, uvRectangle, _texture.id, 0.0f, _colour);
    }

    bool Entity::update()
    {
        _position += _direction * _maxSpeed;

        if (_lifetime > 0 && --_lifetime == 0) {
            return false;
        }
        return true;
    }
}