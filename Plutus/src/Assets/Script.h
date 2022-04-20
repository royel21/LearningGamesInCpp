#pragma once
#include "Asset.h"

namespace Plutus
{
    struct Script : public Asset
    {
        std::string mBuffer;

        Script();

        Script(std::string& path) { init(path); }

        void init(std::string& path);
        void save();
        void destroy();
    };
} // namespace Plutus