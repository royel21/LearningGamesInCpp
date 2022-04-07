#pragma once
#include <string>
#include <unordered_map>

#include <emscripten.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace Plutus
{
    class AudioEvent
    {
    public:
        std::string path;
        uint32_t format;
        uint32_t sampleRate;
        uint32_t source;
        uint32_t buffer;
        uint32_t numberOfSamples;
        uint32_t duration;
        int state = 0;
    };

    using AudioMap = std::unordered_map<std::string, AudioEvent>;

    class AudioEngine
    {
    public:
        static AudioEngine& get();
        ~AudioEngine();
        bool add(const std::string& id, std::string path, int type);
        bool remove(const std::string& id);

        bool play(const std::string& id, bool loop = false);
        bool pause(const std::string& id);
        bool stop(const std::string& id);
        int getState(const std::string& id);

        AudioMap& getItems() { return mSounds; }

        void cleanUp();
    private:
        bool init();

    private:
        AudioEngine();
        AudioMap mSounds;
        ALCcontext* mAudioContext;
        ALCdevice* mAudioDevice;
        bool isInitialized = false;
    };

    extern AudioEngine& SoundEngine;
} // namespace Plutus