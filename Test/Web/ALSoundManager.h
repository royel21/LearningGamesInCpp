#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include "include/alc.h"
#endif

namespace Plutus
{
    class ALSoundManager
    {
    public:
        ALSoundManager();
        ~ALSoundManager();

        void init();
    private:
        ALCcontext* mAudioContext;
        ALCdevice* mAudioDevice;
    };
} // namespace Plutus