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
        Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, Colour colour, std::string entityType);
        ~Entity();

        glm::vec2 getPosition() const { return _position; }
        std::string getEntityType() const { return _entityType; }

        virtual void draw(SpriteBatch& _spriteBatch); ///< Draws entity.
        virtual bool update(); ///< Updates entity.
    protected:
        float _maxSpeed; ///< Entity maximum speed.
        glm::vec2 _direction; /// Entity direction.
        glm::vec2 _position; ///< Entity location.
        glm::vec2 _size;
        int _lifetime; ///< Entity lifetime.
        static GLTexture _texture; ///< Entity texture.
        Colour _colour; ///< Entity colour.
        std::string _entityType;
    };
}