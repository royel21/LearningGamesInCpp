#include "AudioEngine.h"

#include <vector>
#include <Assets/SoundLoader.h>

namespace Plutus
{
    unsigned int getALFormat(unsigned short channels, unsigned short bitsPerSample) {
        if (channels == 1 && bitsPerSample == 8)
            return AL_FORMAT_MONO8;
        else if (channels == 1 && bitsPerSample == 16)
            return AL_FORMAT_MONO16;
        else if (channels == 2 && bitsPerSample == 8)
            return AL_FORMAT_STEREO8;
        else if (channels == 2 && bitsPerSample == 16)
            return AL_FORMAT_STEREO16;
        else {
            return 0;
        }
    }

    AudioEngine& AudioEngine::get() {
        static AudioEngine instance;
        return instance;
    }

    AudioEngine::AudioEngine() {
        isInitialized = init();
    }

    AudioEngine::~AudioEngine() {
        if (isInitialized) {
            cleanUp();
            alcMakeContextCurrent(NULL);
            alcDestroyContext(mAudioContext);
            alcCloseDevice(mAudioDevice);
            isInitialized = false;
        }

    }

    bool AudioEngine::init()
    {
        if (isInitialized) {
            mAudioDevice = alcOpenDevice(nullptr);
            if (!mAudioDevice) {
                return false;
            }

            int attributes[] = { 0 };
            mAudioContext = alcCreateContext(mAudioDevice, attributes);
            if (!mAudioContext) {

                alcCloseDevice(mAudioDevice);
                return false;
            }
            alcMakeContextCurrent(mAudioContext);
        }
        return  true;
    }

    bool AudioEngine::play(const std::string& id, bool loop) {
        if (mSounds.find(id) != mSounds.end()) {
            auto& event = mSounds[id];

            alSourcei(event.source, AL_LOOPING, loop);
            alGetSourcei(event.source, AL_SOURCE_STATE, &event.state);

            if (event.state == AL_PLAYING) {
                alSourceStop(event.source);
            }

            alSourcePlay(event.source);
            event.state = AL_PLAYING;
            return true;
        }
        return false;
    }

    bool AudioEngine::pause(const std::string& id) {
        auto it = mSounds.find(id);
        if (it != mSounds.end()) {
            auto& event = it->second;
            alGetSourcei(event.source, AL_SOURCE_STATE, &event.state);
            if (event.state == AL_PLAYING) {
                alSourcePause(event.source);
                event.state = AL_PAUSED;
                return true;
            }
        }
        return false;
    }

    bool AudioEngine::stop(const std::string& id) {
        auto it = mSounds.find(id);
        if (it != mSounds.end()) {
            auto& event = it->second;
            alGetSourcei(event.source, AL_SOURCE_STATE, &event.state);
            if (event.state == AL_PLAYING || event.state == AL_PAUSED) {
                alSourceStop(event.source);
                event.state = AL_STOPPED;
                return true;
            }
        }
        return false;
    }

    int AudioEngine::getState(const std::string& id)
    {
        auto it = mSounds.find(id);
        if (it != mSounds.end())
            return it->second.state;
        return -1;
    }

    bool AudioEngine::remove(const std::string& id) {
        auto event = mSounds.find(id);
        if (event != mSounds.end()) {
            alSourceStop(event->second.source);
            alDeleteSources(1, &event->second.source);
            alDeleteBuffers(1, &event->second.buffer);

            mSounds.erase(id);
            return true;
        }
        return false;
    }


    bool AudioEngine::add(const std::string& id, std::string path, int type) {

        std::vector<unsigned char> data;
        WaveFormatEx waveformat;

        if (SoundLoader::loadSoundFile(path, &waveformat, &data)) {

            auto& event = mSounds[id];
            event.path = path;
            event.sampleRate = waveformat.nSamplesPerSec;
            event.format = getALFormat(waveformat.nChannels, waveformat.wBitsPerSample);
            event.numberOfSamples = data.size() / (waveformat.nChannels * (waveformat.wBitsPerSample / 8));
            event.duration = event.numberOfSamples / waveformat.nSamplesPerSec;

            if (!event.format) return false;

            alGenBuffers(1, &event.buffer);
            alBufferData(event.buffer, event.format, data.data(), data.size(), (int)waveformat.nSamplesPerSec);

            data.clear();

            alGenSources(1, &event.source);
            alSourcef(event.source, AL_PITCH, 1);
            alSourcef(event.source, AL_GAIN, 1.0f);
            alSource3f(event.source, AL_POSITION, 0, 0, 0);
            alSource3f(event.source, AL_VELOCITY, 0, 0, 0);
            alSourcei(event.source, AL_BUFFER, event.buffer);

            return true;
        }
        return false;
    }

    void AudioEngine::cleanUp() {
        for (auto event : mSounds) {
            alSourceStop(event.second.source);
            alDeleteSources(1, &event.second.source);
            alDeleteBuffers(1, &event.second.buffer);
        }
        mSounds.clear();
    }

    AudioEngine& SoundEngine = AudioEngine::get();
} // namespace Plutus
