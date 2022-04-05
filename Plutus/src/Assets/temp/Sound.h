#pragma once
#include "Asset.h"

namespace Plutus
{
    constexpr int EFFECT = 0;
    constexpr int MUSIC = 0;

    class Sound : public Asset
    {
    public:
        Sound(const std::string& path, int type = EFFECT);

        ~Sound() { destroy(); }

        void play(bool loop = false);
        void pause();
        void stop();

        void destroy() override;
    };
} // namespace Plutus