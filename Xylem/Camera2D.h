#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

namespace Xylem
{
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        void init(int screenWidth, int screenHeight);
        void update();

        glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const;
        //glm::vec2 convertWorldToScreen(glm::vec2 worldCoords) const;

        void setScale(float scale) { _scale = scale; _matrixUpdateRequired = true; }
        void setPosition(glm::vec2& position) { _position = position; _matrixUpdateRequired = true; }

        float getScale() const { return _scale; }
        glm::vec2 getPosition() const { return _position; }
        glm::mat4 getCameraMatrix() const { return _cameraMatrix; }
    private:
        int _screenWidth, _screenHeight;
        bool _matrixUpdateRequired;

        float _scale;
        glm::vec2 _position;
        glm::mat4 _orthographicMatrix;
        glm::mat4 _cameraMatrix;

    };
}
