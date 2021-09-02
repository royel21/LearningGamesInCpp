#pragma once
#include <vector>

namespace Plutus
{
    std::vector<unsigned char> readFile(char* path2File, char* mode = "rb");
} // namespace Plutus