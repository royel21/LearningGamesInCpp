#pragma once
#include <string>

namespace Plutus
{
    constexpr int AssetTexture = 0;
    constexpr int AssetSound = 1;
    constexpr int AssetFont = 3;
    constexpr int AssetScript = 4;

    struct Asset
    {
        std::string mPath;

        virtual void destroy() = 0;
    };
} // namespace Plutus