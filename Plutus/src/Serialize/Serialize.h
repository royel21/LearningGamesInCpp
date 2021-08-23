
#pragma once

#include <rapidjson/prettywriter.h> // for stringify JSON
#include <unordered_map>

namespace Plutus
{
    using namespace rapidjson;

    class Serializer
    {
    public:
        Serializer() : sb(), writer(sb) {}
        PrettyWriter<StringBuffer>* getWriter() { return &writer; }
        const char* getString() { return sb.GetString(); };

    private:
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer;
    };

} // namespace Plutus