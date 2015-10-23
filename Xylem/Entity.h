#pragma once

#include "SpriteBatch.h"
#include "GLTexture.h"
#include "Vertex.h"

#include <GLM\glm.hpp>
#include <string>

namespace Xylem
{
    class Entity
    {
    public:
        Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, Colour colour, float mass, std::string entityType);
        ~Entity();

        glm::vec2 getPosition() const { return _position; }
        glm::vec2 getSize() const { return _size; }
        float getMass() const { return _mass; }
        std::string getEntityType() const { return _entityType; }

        void setPosition(glm::vec2 position) { _position = position; }

        bool isColliding(Entity& entity);

        virtual void draw(SpriteBatch& _spriteBatch); ///< Draws entity.
        virtual bool update(); ///< Updates entity.
    protected:
        float _maxSpeed; ///< Entity maximum speed.
        glm::vec2 _direction; /// Entity direction.
        glm::vec2 _position; ///< Entity location.
        glm::vec2 _size; ///< Entity size.
        int _lifetime; ///< Entity lifetime.
        GLTexture _texture; ///< Entity texture.
        Colour _colour; ///< Entity colour.
        float _mass; ///< Entity mass.
        std::string _entityType;
    };
}