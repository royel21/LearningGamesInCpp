#include "Sound.h"

#ifdef __EMSCRIPTEN__
#include <Platforms/Web/AudioEngine.h>
#elif __ANDROID__
#include <Platforms/Android/AudioEngine.h>
#else
#include <Platforms/Windows/AudioEngine.h>
#include <Platforms/Windows/AudioEvent.h>
#endif

namespace Plutus
{
    Sound::Sound(const std::string& path, int type)
    {
        init(path, type);
    }

    void Sound::init(const std::string& path, int type)
    {
        mPath = path;
        mType = type;

        SoundEngine.remove(path);
        SoundEngine.add(path, path, type);
    }

    void Sound::play(bool loop)
    {
        SoundEngine.play(mPath, loop);
    }

    void Sound::pause()
    {
        SoundEngine.pause(mPath);
    }

    void Sound::stop()
    {
        SoundEngine.stop(mPath);
    }

    int Sound::getState()
    {
        return SoundEngine.getState(mPath);
    }

    void Sound::destroy()
    {
        SoundEngine.remove(mPath);
        mPath = "";
    }
}