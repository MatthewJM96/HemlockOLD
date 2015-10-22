#include "IOManager.h"

#include "ErrorManager.h"

#include <fstream>

namespace Xylem
{
    bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail()) {
            perror(filePath.c_str());
            return false;
        }

        // Seek to the end of the file.
        file.seekg(0, std::ios::end);

        // Get the size of the file.
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        // Account for possible header bytes.
        fileSize -= file.tellg();

        buffer.resize(fileSize);
        file.read((char *)&(buffer[0]), fileSize);

        file.close();

        return true;
    }
}