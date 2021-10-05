#include "AudioEngine.h"

#include <vector>
#include <Assets/SoundLoader.h>

namespace Plutus
{

    AudioEngine& AudioEngine::get() {
        static AudioEngine instance;
        return instance;
    }

    AudioEngine::AudioEngine() {
        isInitialized = init();
    }

    AudioEngine::~AudioEngine() {
        cleanUp();
    }

    bool AudioEngine::init()
    {

        return  true;
    }
    bool AudioEngine::add(std::string id, std::string path, SoundType type) {

        std::vector<u8> data;
        WaveFormatEx waveformat;

        if (SoundLoader::loadSoundFile(path, &waveformat, &data)) {

            return true;
        }
        return false;
    }

    bool remove(std::string id) {
        return false;
    }

    bool AudioEngine::play(std::string id, bool loop) {
        return false;
    }

    bool AudioEngine::pause(std::string id) {
        return false;
    }

    bool AudioEngine::stop(std::string id) {
        return false;
    }

    void AudioEngine::cleanUp() {

    }

    AudioEngine& SoundEngine = AudioEngine::get();
} // namespace Plutus
