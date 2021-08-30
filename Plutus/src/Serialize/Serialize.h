
#pragma once

#include <rapidjson/writer.h> // for stringify JSON
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

} // namespace Plutus