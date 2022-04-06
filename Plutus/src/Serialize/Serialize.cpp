#include "Serialize.h"

#include <fstream>
#include <string>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    void Serializer::addString(const std::string& data)
    {
        writer.String(data.c_str());
    }

    void Serializer::addString(const std::string& id, const std::string& data)
    {
        writer.String(id.c_str());
        writer.String(data.c_str());
    }

    void Serializer::addInt(const std::string& id, int data)
    {

        writer.String(id.c_str());
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

    void Serializer::addFloat(const std::string& id, float data)
    {

        writer.String(id.c_str());
        writer.Double(data);
    }

    void Serializer::addBool(bool data)
    {
        writer.Bool(data);
    }

    void Serializer::addBool(const std::string& id, bool data)
    {
        writer.String(id.c_str());
        writer.Bool(data);
    }

    void Serializer::add2Float(const std::string& id, const vec2f& v)
    {
        StartArr(id);
        {
            writer.Double(v.x);
            writer.Double(v.y);
        }
        EndArr();
    }

    void Serializer::StartObj(const std::string& id)
    {
        writer.String(id.c_str());
        writer.StartObject();
    }

    void Serializer::StartObj()
    {
        writer.StartObject();
    }

    void Serializer::EndObj()
    {
        writer.EndObject();
    }

    void Serializer::StartArr(const std::string& id)
    {
        writer.String(id.c_str());
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

    int PJson::getInt(const char* key, int def) {
        return doc.HasMember(key) ? doc[key].GetInt() : def;
    }

    float PJson::getFloat(const char* key, float def)
    {
        return doc.HasMember(key) ? doc[key].GetFloat() : def;
    }

    bool PJson::getBool(const char* key, bool def)
    {
        return doc.HasMember(key) ? doc[key].GetBool() : def;
    }

    std::string PJson::getString(const char* key, const char* def)
    {
        return doc.HasMember(key) ? doc[key].GetString() : def;
    }
    vec2f PJson::getFloat2(const char* key, vec2f def)
    {
        vec2f f2 = def;
        if (doc.HasMember(key)) {
            auto arr = doc[key].GetArray();
            f2 = { arr[0].GetFloat(), arr[1].GetFloat() };
        }
        return f2;
    }
    vec4f PJson::getFloat4(const char* key, vec4f def)
    {
        vec4f f4 = def;
        if (doc.HasMember(key)) {
            auto arr = doc[key].GetArray();
            f4 = { arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(), arr[3].GetFloat() };
        }
        return f4;
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
