#pragma once
#include "Asset.h"

namespace Plutus
{
    class Script : public Asset
    {
    public:
        Script(std::string& path) { mPath = path; }
        void destroy() override {}
    };
} // namespace Plutus