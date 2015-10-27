#include "Window.h"

#include "ErrorManager.h"

namespace Xylem
{
    Window::Window()
    {
    }

    Window::~Window()
    {
    }

    /// Creates a window instance.
    int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int flags)
    {
        Uint32 sdlFlags = SDL_WINDOW_OPENGL;

        if (flags & HIDDEN) {
            sdlFlags |= SDL_WINDOW_HIDDEN;
        }
        if (flags & FULLSCREEN) {
            sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        if (flags & BORDERLESS) {
            sdlFlags |= SDL_WINDOW_BORDERLESS;
        }

        // Initiliase the SDL Window.
        _sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, sdlFlags);
        if (_sdlWindow == nullptr) {
            ErrorManager::fatalError("SDL Window could not be created!");
        }

        // Initiliase the SDL OpenGL Context.
        SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
        if (glContext == nullptr) {
            ErrorManager::fatalError("SDL_GL context could not be created!");
        }

        // Initialise Glew.
        GLenum error = glewInit();
        if (error != GLEW_OK) {
            ErrorManager::fatalError("Glew could not be initialised!");
        }

        std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

        // Set the clear colour used by OpenGL.
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        // Set VSync on.
        SDL_GL_SetSwapInterval(1);

        // Enable normal alpha blending.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 0;
    }

    /// Swaps window buffer.
    void Window::swapBuffer()
    {
        SDL_GL_SwapWindow(_sdlWindow);
    }
}