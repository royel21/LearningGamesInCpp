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

    private:
        StringBuffer sb;
        Writer<StringBuffer> writer;
    };

    bool loadJson(const char* filePath, rapidjson::Document* doc);

    void toJsonFile(const std::string& path, const char* buffer);

    bool loadJsonFromFile(const char* filePath, rapidjson::Document& doc);
    void saveJsonToFile(const char* filePath, const char* buffer);

    int getInt(rapidjson::Document& doc, char* key);
    float getFloat(rapidjson::Document& doc, char* key);
    bool getBool(rapidjson::Document& doc, char* key);
    std::string getString(rapidjson::Document& doc, char* key);

} // namespace Plutus