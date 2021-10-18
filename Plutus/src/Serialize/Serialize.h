#pragma once

#include <rapidjson/writer.h> // for stringify JSON
#include <rapidjson/document.h>
#include <unordered_map>

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

        int getInt(const char* key);
        float getFloat(const char* key);
        bool getBool(const char* key);
        std::string getString(const char* key);
    };

    bool loadJson(const char* filePath, rapidjson::Document* doc);

    void toJsonFile(const std::string& path, const char* buffer);

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& doc);
    void saveJsonToFile(const char* filePath, const char* buffer);

} // namespace Plutus