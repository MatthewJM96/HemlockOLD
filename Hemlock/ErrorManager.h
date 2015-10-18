#pragma once

#include <string>

class ErrorManager
{
public:
    static void fatalError(std::string message);
};

