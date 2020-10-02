/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <fstream>
#include <sstream>
#include <vector>

namespace utils
{

std::string readFileString(const std::string &filePath);
std::vector<unsigned char> readFile(const char *filePath);

} // namespace utils
