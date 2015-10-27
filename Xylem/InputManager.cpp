#include "InputManager.h"

namespace Xylem
{
    InputManager::InputManager()
    {
    }

    InputManager::~InputManager()
    {
    }

    /// Set the given Key ID as pressed.
    void InputManager::pressKey(unsigned int keyID)
    {
        _keyMap[keyID] = true;
    }

    /// Set the given Key ID as released.
    void InputManager::releaseKey(unsigned int keyID)
    {
        _keyMap[keyID] = false;
    }

    /// Set the mouse coordinates.
    void InputManager::setMouseCoords(float x, float y)
    {
        _mouseCoords.x = x;
        _mouseCoords.y = y;
    }

    /// Checks if a key is pressed.
    bool InputManager::isKeyPressed(unsigned int keyID) const
    {
        auto it = _keyMap.find(keyID);
        if (it != _keyMap.end()) {
            return it->second;
        }
        return false;
    }
}