#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

int InputManager::processInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return 0;
		case SDL_MOUSEMOTION:
			handleMouseMove(&e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {
				handleMouseLeftClick(&e);
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				handleMouseRightClick(&e);
			}
			break;
		}
	}

	return 1;
}

void InputManager::handleMouseMove(SDL_Event* e)
{
	//std::cout << "X: " << e->motion.x << " Y: " << e->motion.y << " XRel: " << e->motion.xrel << " YRel: " << e->motion.yrel << std::endl;
}

void InputManager::handleMouseLeftClick(SDL_Event* e)
{

}

void InputManager::handleMouseRightClick(SDL_Event* e)
{
	//Do something
}