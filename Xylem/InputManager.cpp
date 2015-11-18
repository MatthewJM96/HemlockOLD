#include "InputManager.h"

namespace Xylem
{
    InputManager::InputManager()
    {
    }

    InputManager::~InputManager()
    {
    }
    /// Copy current key map to previous key map.
    void InputManager::update()
    {
        for (auto& it : _keyMap) {
            _previousKeyMap[it.first] = it.second;
        }
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

    /// Checks if a key is currently pressed regardless of frame when the press began.
    bool InputManager::isKeyDown(unsigned int keyID) const
    {
        auto it = _keyMap.find(keyID);
        if (it != _keyMap.end()) {
            return it->second;
        }
        return false;
    }

    /// Checks if a key was pressed this frame.
    bool InputManager::isKeyPressed(unsigned int keyID) const
    {
        if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
            return true;
        }
        return false;
    }

    /// Checks if a key was down the previous frame.
    bool InputManager::wasKeyDown(unsigned int keyID) const
    {
        auto it = _previousKeyMap.find(keyID);
        if (it != _previousKeyMap.end()) {
            return it->second;
        }
        return false;
    }
}