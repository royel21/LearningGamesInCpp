#pragma once
#include <vector>
#include <string>

namespace Plutus
{
    std::vector<unsigned char> readFile(const char* path2File, const char* mode = "rb");

    std::string readFileAsString(const char* path);
} // namespace Plutus