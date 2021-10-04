#pragma once
#include <string>
#include <unordered_map>

namespace Plutus
{
    struct Audio
    {

    };

    class Audios
    {
    public:
        std::unordered_map<std::string, Audio> mAudios;
    public:
        Audios();
    };
} // namespace Plutus