#pragma once
#include <string>
#include <unordered_map>

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

    enum SoundType
    {
        MUSIC,
        EFFECT
    };

    class AudioEngine
    {
    public:
        static AudioEngine& get();
        ~AudioEngine();
        bool add(std::string name, std::string path, SoundType type);
        void remove(std::string name);

        bool play(std::string name, bool loop = false);
        bool pause(std::string name);
        bool stop(std::string name);

        AudioMap& getItems() { return mSounds; }

        void cleanUp();
    private:
        bool init();

    private:
        AudioEngine();
        AudioMap mSounds;
    };

    extern AudioEngine& SoundEngine;
} // namespace Plutus