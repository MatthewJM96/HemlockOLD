#include "Camera2D.h"

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
        // Invert Y direction.
        screenCoords.y = _screenHeight - screenCoords.y;
        // Make (0,0) centre of screen.
        screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
        // Scale the coordinates.
        screenCoords /= _scale;
        // Translate with the camera position.
        screenCoords += _position;

        return screenCoords;
    }

    //glm::vec2 Xylem::Camera2D::convertWorldToScreen(glm::vec2 worldCoords) const
    //{
    //    // Translate to negate camera position.
    //    worldCoords -= _position;
    //    // Scale the coordinates.
    //    worldCoords *= _scale;
    //    // Make (0,0) centre of screen.
    //    worldCoords += glm::vec2(_screenWidth / 2, _screenHeight / 2);
    //    // Invert Y direction.
    //    worldCoords.y = worldCoords.y - _screenHeight;

    //    return worldCoords;
    //}
}