#pragma once
#include <string>

namespace Plutus
{
    enum AssetType {
        Texture,
        Sound,
        Font,
        Script
    };

    struct Asset
    {
    public:
        std::string path;
        AssetType type;

        virtual void destroy() = 0;
    };
} // namespace Plutus