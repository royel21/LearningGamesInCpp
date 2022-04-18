#pragma once
#include <vector>
#include <string>

namespace Plutus
{
    std::vector<unsigned char> readFile(const char* path2File, const char* mode = "rb");

    std::string readFileAsString(const char* path);

    void saveBufferToFile(const std::string& path, const char* buffer);

    bool exists(const char* path);
} // namespace Plutus