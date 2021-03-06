#pragma once
#include "Asset.h"

namespace Plutus
{
    struct Script : public Asset
    {
        std::string mBuffer;

        Script() = default;

        Script(const std::string& path) { init(path); }

        void init(const std::string& path);
        void save(const std::string& buffe = "");
        void destroy();
    };
} // namespace Plutus