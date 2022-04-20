#pragma once
#include "Asset.h"

namespace Plutus
{
    struct SceneAsset : public Asset
    {
        SceneAsset() = default;
        SceneAsset(const std::string& path) { init(path); }
        void init(const std::string& path);

        void destroy() override { };
    };
} // namespace Plutus