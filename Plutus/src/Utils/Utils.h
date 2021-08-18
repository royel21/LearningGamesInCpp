#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "rapidjson/document.h"

#define SAVE_FILE 0
#define OPEN_FILE 1

#define LIMIT(v, min, max) v<min ? max : v> max ? min : v

constexpr uint32_t COLOR_RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return a << 24 | b << 16 | g << 8 | r;
}

namespace Plutus
{
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long ulong;

    namespace Utils
    {
        std::string getFileName(const std::string& filePath);
        //Get file extenxion
        std::string getExtension(const std::string& fileName);

        bool loadJson(const char* filePath, rapidjson::Document* document);

        void toJsonFile(const std::string& path, const char* buffer);

        std::vector<std::string> listFiles(const std::string& path, const std::string& fileType);

        int getIndex(const std::vector<std::string>& vec, const std::string& s);
    } // namespace Utils

} // namespace Plutus