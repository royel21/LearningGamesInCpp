#include "Sound.h"
#include "../SoundEngine.h"

namespace Plutus
{
    Sound::Sound(const std::string& path, int type)
    {
        mPath = path;
        mType = AssetSound;
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

    void Sound::destroy()
    {
        SoundEngine.remove(mPath);
    }
}