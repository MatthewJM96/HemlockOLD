#pragma once

#include "SpriteBatch.h"
#include "GLTexture.h"
#include "Vertex.h"
#include "Camera2D.h"

#include <GLM\glm.hpp>
#include <string>

namespace Xylem
{
    class Entity
    {
    public:
        Entity(float maxSpeed, glm::vec2 initialDirection, glm::vec2 initialPosition, glm::vec2 size, int lifetime, GLTexture texture, ColourRGBA8 colour, float mass, std::string entityType);
        ~Entity();

        glm::vec2 getPosition() const { return _position; }
        glm::vec2 getSize() const { return _size; }
        float getMass() const { return _mass; }
        std::string getEntityType() const { return _entityType; }

        void setPosition(glm::vec2 position) { _position = position; }

        bool collideWithEntity(Entity& entity);

        virtual void draw(SpriteBatch& spriteBatch, const Camera2D& camera); ///< Draws entity.
        virtual bool update(float deltaTime); ///< Updates entity.
    protected:
        float _maxSpeed; ///< Entity maximum speed.
        glm::vec2 _direction; /// Entity direction.
        glm::vec2 _position; ///< Entity location.
        glm::vec2 _size; ///< Entity size.
        int _lifetime; ///< Entity lifetime.
        GLTexture _texture; ///< Entity texture.
        ColourRGBA8 _colour; ///< Entity colour.
        float _mass; ///< Entity mass.
        std::string _entityType; ///< Entity type.
    };
}