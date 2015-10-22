#include "Xylem.h"

#include <SDL\SDL.h>
#include <GL\glew.h>

namespace Xylem
{
    int init() {
        // Initialise SDL.
        SDL_Init(SDL_INIT_EVERYTHING);

        // Set OpenGL to use double buffering.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        return 0;
    }
}