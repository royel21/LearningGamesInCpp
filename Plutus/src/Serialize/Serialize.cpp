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

    void Serializer::add2Float(const std::string& id, const Vec2f& v)
    {
        StartArr(id);
        {
            writer.Double(v.x);
            writer.Double(v.y);
        }
        EndArr();
    }

    void Serializer::add4Float(const std::string& id, const Vec4f& v)
    {
        StartArr(id);
        {
            writer.Double(v.x);
            writer.Double(v.y);
            writer.Double(v.z);
            writer.Double(v.w);
        }
        EndArr();
    }

    void Serializer::StartObj(const std::string& id)
    {
        if (!id.empty())
            writer.String(id.c_str());
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

/********************************************************************/

    int JsonHelper::getInt(const char* key, int def) {
        return value->HasMember(key) ? (*value)[key].GetInt() : def;
    }

    float JsonHelper::getFloat(const char* key, float def)
    {
        return value->HasMember(key) ? (*value)[key].GetFloat() : def;
    }

    bool JsonHelper::getBool(const char* key, bool def)
    {
        return value->HasMember(key) ? (*value)[key].GetBool() : def;
    }

    std::string JsonHelper::getString(const char* key, const char* def)
    {
        return value->HasMember(key) ? (*value)[key].GetString() : def;
    }

    Plutus::Vec2f JsonHelper::getFloat2(const char* key, Plutus::Vec2f def)
    {
        Plutus::Vec2f f2 = def;
        if (value->HasMember(key)) {
            auto arr = (*value)[key].GetArray();
            f2 = { arr[0].GetFloat(), arr[1].GetFloat() };
        }
        return f2;
    }
    Plutus::Vec4f JsonHelper::getFloat4(const char* key, Plutus::Vec4f def)
    {
        Plutus::Vec4f f4 = def;
        if (value->HasMember(key)) {
            auto arr = (*value)[key].GetArray();
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
            auto buffer = FileIO::readFileAsString(filePath);
            if (!buffer.empty())
            {
                bool result = document->Parse(buffer.c_str()).HasParseError() == false;
                return result;
            }
        }
        return false;
    }

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& document) {
        std::string ex = Utils::getExtension(filePath);
        if (ex == "json")
        {
            auto buffer = FileIO::readFileAsString(filePath);
            if (!buffer.empty())
            {
                return document.Parse(buffer.c_str()).HasParseError() == false;
            }
        }
        return false;
    }
} // namespace Plutus
