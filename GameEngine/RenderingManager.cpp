#include "RenderingManager.h"
#include <GL/glew.h>
#include <string>
#include "Vec2D.h"
#include "ErrorManager.h"

RenderingManager::RenderingManager(SDL_Window* window)
	: _window(window), _isInit(false), _time(0)
{
}

RenderingManager::~RenderingManager()
{
}

void RenderingManager::initialiseRenderer()
{
	_testSprites.push_back(new Sprite());
	_testSprites[_testSprites.size() - 1]->init(Vec2D <float>(-1.0f, -1.0f), Vec2D <float>(2.0f, 2.0f));
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	initialiseShaders();

	_isInit = true;
}

int RenderingManager::render(GLuint time)
{
	if (!_isInit) {
		return 0;
	}

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colourShadingProgram.use();

	glUniform1f(_time, ((float)time));

	for (size_t i = 0; i < _testSprites.size(); ++i) {
		if (!_testSprites[i]->draw()) {
			ErrorManager::error("Test Sprite could not be drawn!", ErrorLevel::MEDIUM, false);
		}
	}

	_colourShadingProgram.unuse();

	SDL_GL_SwapWindow(_window);

	return 1;
}

void RenderingManager::initialiseShaders()
{
	_colourShadingProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
	_colourShadingProgram.addAttribute("vertexPosition");
	_colourShadingProgram.addAttribute("vertexColour");
	_colourShadingProgram.linkShaders();
	_time = _colourShadingProgram.getUniformLocation("deltaTime");
	if (_time == -1) {
		ErrorManager::error("Uniform variable, delta time, could not be found!", ErrorLevel::FATAL, false);
	}
}