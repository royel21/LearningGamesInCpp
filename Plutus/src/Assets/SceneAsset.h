#pragma once
#include "Asset.h"

namespace Plutus
{
    struct SceneAsset : public Asset
    {
        std::string mBuffer;

        SceneAsset() = default;
        SceneAsset(const std::string& path) { init(path); }

        void init(const std::string& path);

        void save();

        void destroy();
    };
} // namespace Plutus