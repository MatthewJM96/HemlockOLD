#pragma once
#include <SDL/SDL.h>
#include <vector>
#include "GLSLProgram.h"
#include "Sprite.h"
class RenderingManager
{
public:
	RenderingManager(SDL_Window* window);
	~RenderingManager();
	int render(GLuint time);
	void initialiseRenderer();
private:
	void initialiseShaders();

	SDL_Window* _window;
	bool _isInit;

	GLSLProgram _colourShadingProgram;
	GLint _time;

	std::vector<Sprite*> _testSprites;
};

