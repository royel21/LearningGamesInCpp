#pragma once

#include <rapidjson/writer.h> // for stringify JSON
#include <rapidjson/document.h>
#include <unordered_map>
#include <Math/Vectors.h>

namespace Plutus
{
    using namespace rapidjson;

    class Serializer
    {
    public:
        Serializer() : sb(), writer(sb) {}
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

        void add2Float(const std::string& id, const vec2f& v);

        void StartObj(const std::string& id);

        void StartObj();
        void EndObj();

        void StartArr(const std::string& id);
        void EndArr();

    private:
        StringBuffer sb;
        Writer<StringBuffer> writer;
    };

    struct PJson {
        bool isLoaded = false;
        rapidjson::Document doc;

        PJson() = default;
        PJson(const char* path);
        bool load(const char* path);

        int getInt(const char* key, int def = 0);
        float getFloat(const char* key, float def = 0);
        bool getBool(const char* key, bool def = 0);
        std::string getString(const char* key, const char* def = "");

        vec2f getFloat2(const char* key, vec2f def = { 0 });
        vec4f getFloat4(const char* key, vec4f def = { 0 });
    };

    bool loadJson(const char* filePath, rapidjson::Document* doc);

    void toJsonFile(const std::string& path, const char* buffer);

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& doc);
    void saveJsonToFile(const char* filePath, const char* buffer);

} // namespace Plutus