#pragma once

#include <cstdint>

namespace Plutus
{
    class Batcher
    {
    public:
        Batcher();
        ~Batcher();
        void init();

    private:
        uint32_t mVAO;
        uint32_t mBuffId;
    };
} // namespace Plutus