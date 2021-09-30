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

    bool loadJson(const char* filePath, rapidjson::Document* document);

    void toJsonFile(const std::string& path, const char* buffer);

} // namespace Plutus