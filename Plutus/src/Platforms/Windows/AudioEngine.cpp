#include "AudioEngine.h"

#include "AudioEvent.h"

namespace Plutus
{
	bool AudioEngine::has_COINIT = false;

	AudioEngine::AudioEngine() : mXAuido2(NULL), mMasterVoice(NULL)
	{
		Init();
	}

	AudioEngine& AudioEngine::get()
	{
		has_COINIT = true;
		static AudioEngine instance;
		return instance;
	}

	AudioEngine::~AudioEngine()
	{
		cleanUp();

		if (mMasterVoice)
		{
			mMasterVoice->DestroyVoice();
		}
		if (mXAuido2)
		{
			mXAuido2->StopEngine();
			mXAuido2->Release();
		}
		if (has_COINIT)
			CoUninitialize();
	}
	bool AudioEngine::Init()
	{
		if (has_COINIT)
		{
			if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
				return 0;
		}

		HRESULT hr = S_OK;

		hr = XAudio2Create(&mXAuido2);
		if (FAILED(hr))
		{
			return false;
		}

		hr = mXAuido2->CreateMasteringVoice(&mMasterVoice);
		if (FAILED(hr))
		{
			return false;
		}

		mXAuido2->CreateSubmixVoice(&mSoundsSubmix, 1, 44100, 0, 0, 0, 0);
		mXAuido2->CreateSubmixVoice(&mMusicSubmix, 1, 44100, 0, 0, 0, 0);

		sendSounds = { 0, mSoundsSubmix };
		sendMusic = { 0, mMusicSubmix };

		mSoundsSendList = { 1, &sendSounds };
		mMusicSendList = { 1, &sendMusic };

		return true;
	}

	AudioEvent* AudioEngine::createEvent(const std::string& name, const std::string& path, SoundType type) {
		auto event = new AudioEvent(name, path);

		if (!event->mAudioData.size()) return nullptr;

		HRESULT hr = S_OK;
		if (type == MUSIC)
		{
			hr = mXAuido2->CreateSourceVoice(
				&event->mSourceVoice,
				&event->mWaveFormat.Format,
				0, XAUDIO2_DEFAULT_FREQ_RATIO,
				nullptr,
				&mSoundsSendList,
				NULL);
		}
		else
		{
			hr = mXAuido2->CreateSourceVoice(
				&event->mSourceVoice,
				&event->mWaveFormat.Format,
				0, XAUDIO2_DEFAULT_FREQ_RATIO,
				nullptr,
				&mMusicSendList,
				NULL);
		}

		if (FAILED(hr)) return nullptr;

		return event;
	}
	bool AudioEngine::add(const std::string& name, std::string path, SoundType type)
	{

		if (!mXAuido2 || !mMasterVoice || mSounds.find(name) != mSounds.end())
			return false;
		auto event = createEvent(name, path, type);
		if (event) {
			mSounds[name] = event;
			return true;
		}
		return false;
	}

	void AudioEngine::remove(const std::string& name)
	{
		auto& aEvent = mSounds[name];
		if (aEvent)
		{
			aEvent->mSourceVoice->Stop();
			delete aEvent;
			mSounds.erase(name);
		}
	}

	bool AudioEngine::play(AudioEvent* event, bool loop) {
		if (event)
		{
			XAUDIO2_VOICE_STATE aState;
			event->mSourceVoice->GetState(&aState);

			if (aState.BuffersQueued)
			{
				event->mSourceVoice->Stop();
				event->mSourceVoice->FlushSourceBuffers();
			}

			event->mAudioBuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

			if (event)
				return event->start();
		}
		return false;
	}

	bool AudioEngine::pause(AudioEvent* event) {
		return event->pause();
	}

	bool AudioEngine::stop(AudioEvent* event) {
		return event->stop();
	}

	bool AudioEngine::pause(const std::string& name)
	{
		if (mSounds.find(name) != mSounds.end())
			return mSounds[name]->pause();

		return false;
	}

	bool AudioEngine::stop(const std::string& name)
	{
		if (mSounds.find(name) != mSounds.end())
			return mSounds[name]->stop();
		return false;
	}

	void AudioEngine::cleanUp()
	{
		for (auto& itr : mSounds)
		{
			delete itr.second;
		}
		mSounds.clear();
	}

	AudioEngine& SoundEngine = AudioEngine::get();
} // namespace Plutus