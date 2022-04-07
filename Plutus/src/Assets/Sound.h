#pragma once
#include "Asset.h"

namespace Plutus
{
    constexpr int EFFECT = 0;
    constexpr int MUSIC = 0;

    struct Sound : public Asset
    {
        int mType = EFFECT;

        Sound() = default;
        Sound(const std::string& path, int type = EFFECT);

        ~Sound() {
            destroy();
        }
        void init(const std::string& path, int type = EFFECT);
        void play(bool loop = false);
        void pause();
        void stop();

        int getState();

        void destroy() override;

    };
} // namespace Plutus