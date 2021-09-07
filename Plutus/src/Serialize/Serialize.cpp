#include "Serialize.h"

#include <fstream>
#include <string>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    bool loadJson(const char* filePath, rapidjson::Document* document)
    {
        std::string ex = Utils::getExtension(filePath);
        if (ex == "json")
        {
            auto buffer = readFileAsString(filePath);
            if (!buffer.empty())
            {
                bool result = document->Parse(buffer.c_str()).HasParseError() == false;
                return result;
            }
        }
        return false;
    }

    void toJsonFile(const std::string& path, const char* buffer)
    {
        std::ofstream outfile;
        outfile.open(path.c_str(), std::ios_base::out); //std::ios_base::app
        outfile << buffer;
        outfile.close();
    }
} // namespace Plutus
