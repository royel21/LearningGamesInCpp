#include "Sound.h"
#include "AssetManager.h"

#include <miniaudio.h>
#include <Log/Logger.h>

namespace Plutus
{
    Sound::Sound(const std::string& path, int type)
    {
        init(path, type);
    }

    void Sound::init(const std::string& path, int type)
    {
        if (AssetManager::get()->mAudioEngine == nullptr) {
            Logger::error("Audio engine is not initialized");
            return;
        }

        mPath = path;
        mType = type;

        if (mSound != nullptr) {
            delete mSound;
        }

        mSound = new ma_sound;

        ma_uint32 flags = 0;

        if (type) {
            flags = MA_SOUND_FLAG_STREAM;
        }

        ma_result result = ma_sound_init_from_file(AssetManager::get()->mAudioEngine, path.c_str(), flags, NULL, NULL, mSound);

        if (result != MA_SUCCESS) {
            delete mSound;
            mSound = nullptr;
            Logger::error("Failed load sound path: %s", path.c_str());
            return;
        }
    }

    void Sound::play(bool loop)
    {
        if (mSound != nullptr) {
            ma_sound_set_looping(mSound, loop);
            if (!ma_sound_is_playing(mSound)) {
                ma_sound_start(mSound);
            }
        }
    }

    void Sound::pause()
    {
        if (mSound != nullptr) {
            if (ma_sound_is_playing(mSound)) {
                ma_sound_stop(mSound);
            }
        }
    }

    void Sound::stop()
    {
        if (mSound != nullptr) {
            if (ma_sound_is_playing(mSound)) {
                ma_sound_stop(mSound);
            }
        }
    }

    int Sound::getState()
    {
        return mSound == nullptr ? false : ma_sound_is_playing(mSound);
    }

    void Sound::destroy()
    {
        if (mSound != nullptr) {
            if (ma_sound_is_playing(mSound)) {
                ma_sound_stop(mSound);
            }
            ma_sound_uninit(mSound);

            delete mSound;
            mSound = nullptr;
        }
        mPath = "";
    }
}