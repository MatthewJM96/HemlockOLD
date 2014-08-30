#pragma once
#include <SDL/SDL.h>
#include <iostream>
class InputManager
{
public:
	InputManager();
	~InputManager();
	int processInput();
private:
	void handleMouseMove(SDL_Event* e);
	void handleMouseLeftClick(SDL_Event* e);
	void handleMouseRightClick(SDL_Event* e);
};

