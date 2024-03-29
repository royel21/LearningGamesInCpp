#pragma once

#include <string>
#include <vector>
#include <iostream>

#define LIMIT(v, min, max) v<min ? max : v> max ? min : v

namespace Plutus
{
    namespace Utils
    {
        std::string getDirectory(const std::string& filePath);

        std::string getFileName(const std::string& filePath);
        //Get file extenxion
        std::string getExtension(const std::string& fileName);

        std::vector<std::string> listFiles(const std::string& path, const std::string& fileType);

        int getIndex(const std::vector<std::string>& vec, const std::string& s);

        bool createFile(const char* file, const char* content, bool asString = true);

        void saveFile(const char* filePath, const char* buffer);

        void replaceAll(std::string& str, char what, char with);

        std::string ToLowerCase(const std::string& str);

        int getRandom(int min, int max);
    } // namespace Utils

} // namespace Plutus