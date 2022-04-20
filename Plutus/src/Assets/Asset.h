#pragma once
#include <string>

namespace Plutus
{
    constexpr int AssetTexture = 0;
    constexpr int AssetSound = 1;
    constexpr int AssetFont = 3;

    struct Asset
    {
        std::string mPath;
        std::string baseDir = "";
        virtual ~Asset() {};

        virtual void destroy() = 0;

        void setDir(const std::string dir) { baseDir = dir; }
    };
} // namespace Plutus