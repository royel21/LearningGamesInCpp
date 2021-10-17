#pragma once

#include <string>
#include <vector>
#include <iostream>

#define LIMIT(v, min, max) v<min ? max : v> max ? min : v
#define float2ubyte(c) static_cast<uint8_t>(c * 255)
#define RGBA2Int(r, g, b, a) a << 24 | b << 16 | g << 8 | r

namespace Plutus
{
    namespace Utils
    {
        std::string getFileName(const std::string& filePath);
        //Get file extenxion
        std::string getExtension(const std::string& fileName);

        std::vector<std::string> listFiles(const std::string& path, const std::string& fileType);

        int getIndex(const std::vector<std::string>& vec, const std::string& s);
    } // namespace Utils

} // namespace Plutus