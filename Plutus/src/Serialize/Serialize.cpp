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

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& document) {
        bool result;
        std::string ex = Utils::getExtension(filePath);
        if (ex == "json")
        {
            auto buffer = readFileAsString(filePath);
            if (!buffer.empty())
            {
                bool result = document.Parse(buffer.c_str()).HasParseError() == false;
                return result;
            }
        }
        return false;
    }

    void saveJsonToFile(const char* filePath, const char* buffer) {
        std::ofstream outfile;
        outfile.open(filePath, std::ios_base::out); //std::ios_base::app
        outfile << buffer;
        outfile.close();
    }

    int getInt(rapidjson::Document& doc, char* key) {
        return doc.HasMember(key) ? doc[key].GetInt() : 0;
    }
    float getFloat(rapidjson::Document& doc, char* key) {
        return doc.HasMember(key) ? doc[key].GetFloat() : 0.0f;
    }
    bool getBool(rapidjson::Document& doc, char* key) {
        return doc.HasMember(key) ? doc[key].GetBool() : false;
    }
    std::string getString(rapidjson::Document& doc, char* key) {
        return doc.HasMember(key) ? doc[key].GetString() : "";
    }
} // namespace Plutus
