#include "ALSoundManager.h"
#include <string>

namespace Plutus
{
    void ALSoundManager::init()
    {
        auto defDevice = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
        mAudioDevice = alcOpenDevice(defDevice);

        int attributes[] = { 0 };
        mAudioContext = alcCreateContext(mAudioDevice, attributes);
        alcMakeContextCurrent(mAudioContext);

        ALCCapabilities alcCapabilities = createCapabilities(mAudioDevice);
        ALCapabilities alCapabilities = createCapabilities(alcCapabilities);

        if (!alCapabilities.OpenAL10) {
            assert false : "Audio library not supported.";
        }
    }
}