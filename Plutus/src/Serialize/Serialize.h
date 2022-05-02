#pragma once

#include <rapidjson/writer.h> // for stringify JSON
#include <rapidjson/document.h>

#include <string>
#include <unordered_map>

#include <Math/Vectors.h>


#define JSONVVALUE rapidjson::GenericObject<false, rapidjson::Value>

namespace Plutus
{
    using namespace rapidjson;

    class Serializer
    {
    public:
        Serializer() : sb(), writer(sb) {
            writer.SetMaxDecimalPlaces(4);
        }
        Writer<StringBuffer>* getWriter() { return &writer; }
        const char* getString() { return sb.GetString(); };

        void addString(const std::string& data);
        void addInt(int data);
        void addFloat(float data);
        void addBool(bool data);

        void addString(const std::string& id, const std::string& data);
        void addInt(const std::string& id, int data);
        void addFloat(const std::string& id, float data);
        void addBool(const std::string& id, bool data);

        void add2Float(const std::string& id, const Vec2f& v);

        void StartObj(const std::string& id = "");
        void EndObj();

        void StartArr(const std::string& id);
        void EndArr();

    private:
        StringBuffer sb;
        Writer<StringBuffer> writer;
    };

    struct JsonHelper {
        JSONVVALUE* value;

        JsonHelper() = default;

        int getInt(const char* key, int def = 0);
        float getFloat(const char* key, float def = 0);
        bool getBool(const char* key, bool def = 0);
        std::string getString(const char* key, const char* def = "");

        Vec2f getFloat2(const char* key, Vec2f def = { 0 });
        Vec4f getFloat4(const char* key, Vec4f def = { 0 });
    };

    bool loadJson(const char* filePath, rapidjson::Document* doc);

    void toJsonFile(const std::string& path, const char* buffer);

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& doc);
} // namespace Plutus