#include "ErrorManager.h"

#include <SDL/SDL.h>

#include <iostream>
#include <cstdlib>

namespace Xylem
{
    void ErrorManager::fatalError(std::string message)
    {
        std::cout << message << std::endl;

        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;

        SDL_Quit();
        exit(1);
    }

    void ErrorManager::error(std::string message)
    {
        std::cout << message << std::endl;
    }
}