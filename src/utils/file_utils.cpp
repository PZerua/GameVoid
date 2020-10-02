/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include "file_utils.h"

#include <iostream>

namespace utils
{

std::string readFileString(const std::string &filePath)
{
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open())
    {
        std::cout << "Error opening: " << filePath << std::endl;
        return "";
    }

    std::stringstream fileStringstream;
    fileStringstream << fileStream.rdbuf();

    fileStream.close();

    return fileStringstream.str();
}

std::vector<unsigned char> readFile(const char *filePath)
{
    std::ifstream fileStream(filePath, std::ios::ate);

    unsigned size = (int)fileStream.tellg();

    if (!fileStream.is_open())
    {
        std::cout << "Error opening: " << filePath << std::endl;
        return {};
    }

    // Points to the beginning of the file
    fileStream.seekg(0, std::ios::beg);

    std::vector<unsigned char> data(size);

    fileStream.read((char *)&data[0], size);

    fileStream.close();

    return data;
}

} // namespace utils
