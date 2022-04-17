#pragma once
#include "Asset.h"

struct ma_sound;

namespace Plutus
{
    constexpr int EFFECT = 0;
    constexpr int MUSIC = 1;

    struct Sound : public Asset
    {
    private:
        ma_sound* mSound = nullptr;

    public:
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