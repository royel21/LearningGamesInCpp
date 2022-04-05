#pragma once
#include "Asset.h"

namespace Plutus
{
    class Script : public Asset
    {
    public:
        Script();
        void destroy() override {}
    };
} // namespace Plutus