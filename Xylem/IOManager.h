#pragma once

#include <string>
#include <vector>

namespace Xylem
{
    class IOManager
    {
    public:
        static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
        static bool readFileLinesToBuffer(std::string filePath, std::vector<std::string>& buffer);
    };
}