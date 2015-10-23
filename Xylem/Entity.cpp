#include "Entity.h"

#include <iostream>

namespace Xylem
{
    Entity::Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, Colour colour, float mass, std::string entityType)
        : _maxSpeed(maxSpeed), _direction(initialDirection), _position(initialPosition), _size(size), _lifetime(lifetime), _texture(texture), _colour(colour), _mass(mass), _entityType(entityType)
    {
    }

    Entity::~Entity()
    {
    }

    bool Entity::collideWithEntity(Entity& entity)
    {
        float averageSizeA = (_size.x + _size.y) / 2.0f;
        float averageSizeB = (entity.getSize().x + entity.getSize().y) / 2.0f;
        float distanceToCollide = (averageSizeA / 2.0f) + (averageSizeB / 2.0f);

        glm::vec2 centreOfA = _position + (_size / 2.0f);
        glm::vec2 centreOfB = entity.getPosition() + (entity.getSize() / 2.0f);

        glm::vec2 rVect = centreOfA - centreOfB;

        float distance = glm::length(rVect);

        float collisionDepth = distanceToCollide - distance;

        if (collisionDepth > 0) {
            glm::vec2 reactionVect = glm::normalize(rVect) * collisionDepth;

            float relativeDeltaA = entity.getMass() / (_mass + entity.getMass());
            float relativeDeltaB = _mass / (_mass + entity.getMass());

            _position += reactionVect * relativeDeltaA;
            entity.setPosition(entity.getPosition() - (reactionVect * relativeDeltaB));
            return true;
        }
        return false;
    }

    void Entity::draw(SpriteBatch& spriteBatch, const Camera2D& camera)
    {
        if (!camera.isOnScreen(_position)) {
            return;
        }

        glm::vec4 uvRectangle(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 destinationRectangle(_position.x, _position.y, _size.x, _size.y);

        spriteBatch.draw(destinationRectangle, uvRectangle, _texture.id, 0.0f, _colour);
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