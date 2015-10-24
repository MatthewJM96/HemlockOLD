#include "Camera2D.h"

#include <iostream>

namespace Xylem
{
    Camera2D::Camera2D()
        : _screenWidth(1280), _screenHeight(720), _matrixUpdateRequired(true), _scale(1.0f), _position(0.0f, 0.0f), _orthographicMatrix(1.0f), _cameraMatrix(1.0f)
    {
    }

    Camera2D::~Camera2D()
    {
    }

    void Camera2D::init(int screenWidth, int screenHeight)
    {
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;

        _orthographicMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
    }

    void Camera2D::update()
    {
        if (_matrixUpdateRequired) {
            glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
            _cameraMatrix = glm::translate(_orthographicMatrix, translate);

            glm::vec3 scale(_scale, _scale, 0.0f);
            _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

            _matrixUpdateRequired = false;
        }
    }
    
    glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) const
    {
        // Invert Y.
        glm::vec2 worldCoords = glm::vec2(screenCoords.x, -1.0f * screenCoords.y);
        // Make (0,0) centre of screen.
        worldCoords -= glm::vec2(_screenWidth / 2.0f, _screenHeight / -2.0f);
        // Scale the coordinates.
        worldCoords /= _scale;
        // Translate with the camera position.
        worldCoords += _position;

        return worldCoords;
    }

    glm::vec2 Camera2D::convertWorldToScreen(glm::vec2 worldCoords) const
    {
        glm::vec2 screenCoords = worldCoords;
        // Translate to negate camera position.
        screenCoords -= _position;
        // Scale the coordinates.
        screenCoords *= _scale;
        // Make (0,0) centre of screen.
        screenCoords += glm::vec2(_screenWidth / 2.0f, _screenHeight / 2.0f);
        // Invert Y.
        screenCoords = glm::vec2(screenCoords.x, -1.0f * screenCoords.y);

        return screenCoords;
    }

    bool Camera2D::isOnScreen(glm::vec2 worldCoords, glm::vec2 size) const
    {
        glm::vec2 screenCoords = convertWorldToScreen(worldCoords);
    
        if (screenCoords.x > (-1.0f * size.x) && screenCoords.x < (_screenWidth + size.x)) {
            if (screenCoords.y < size.y && screenCoords.y > (-_screenHeight - size.y)) {
                return true;
            }
        }

        return false;
    }
}