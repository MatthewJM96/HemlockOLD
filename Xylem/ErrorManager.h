#pragma once

#include <string>

namespace Xylem
{
    class ErrorManager
    {
    public:
        static void fatalError(std::string message);
        static void error(std::string message);
    };
}