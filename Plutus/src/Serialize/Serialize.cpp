#include "Serialize.h"

#include <fstream>
#include <string>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    void Serializer::addString(const char* data)
    {
        writer.String(data);
    }

    void Serializer::addString(const char* id, const char* data)
    {
        writer.String(id);
        writer.String(data);
    }

    void Serializer::addInt(const char* id, int data)
    {

        writer.String(id);
        writer.Int(data);
    }
    void Serializer::addInt(int data)
    {
        writer.Int(data);
    }

    void Serializer::addFloat(float data)
    {
        writer.Double(data);
    }

    void Serializer::addFloat(const char* id, float data)
    {

        writer.String(id);
        writer.Double(data);
    }

    void Serializer::addBool(bool data)
    {
        writer.Bool(data);
    }

    void Serializer::addBool(const char* id, bool data)
    {
        writer.String(id);
        writer.Bool(data);
    }

    void Serializer::StartObj()
    {
        writer.StartObject();
    }
    void Serializer::EndObj()
    {
        writer.EndObject();
    }

    void Serializer::StartArr(const char* id)
    {
        writer.String(id);
        writer.StartArray();
    }

    void Serializer::EndArr()
    {
        writer.EndArray();
    }
    /********************************************************************/
    PJson::PJson(const char* path) {
        isLoaded = load(path);
    }

    bool PJson::load(const char* path)
    {
        return isLoaded = loadJsonFromFile(path, doc);
    }

    int PJson::getInt(const char* key) {
        return doc.HasMember(key) ? doc[key].GetInt() : 0;
    }

    float PJson::getFloat(const char* key)
    {
        return doc.HasMember(key) ? doc[key].GetFloat() : 0.0f;
    }

    bool PJson::getBool(const char* key)
    {
        return doc.HasMember(key) ? doc[key].GetBool() : false;
    }

    std::string PJson::getString(const char* key)
    {
        return doc.HasMember(key) ? doc[key].GetString() : "";
    }
    /********************************************************************/
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
        std::string ex = Utils::getExtension(filePath);
        if (ex == "json")
        {
            auto buffer = readFileAsString(filePath);
            if (!buffer.empty())
            {
                return document.Parse(buffer.c_str()).HasParseError() == false;
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
} // namespace Plutus
