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

	bool AudioEngine::add(std::string name, std::string path, SoundType type)
	{
		if (!mXAuido2 || !mMasterVoice || mSounds[name])
			return false;

		auto event = new AudioEvent(name, path);

		if (!event->mAudioData.size())
		{
			return false;
		}
		event->type = type;
		mSounds[name] = event;
		HRESULT hr = S_OK;
		// create source voice
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
		if (FAILED(hr))
		{
			return 1;
		}
		return 0;
	}

	void AudioEngine::remove(std::string name)
	{
		auto& aEvent = mSounds[name];
		if (aEvent)
		{
			aEvent->mSourceVoice->Stop();
			delete aEvent;
			mSounds.erase(name);
		}
	}

	bool AudioEngine::play(std::string name, bool loop)
	{
		auto aEvent = mSounds[name];
		if (aEvent)
		{
			XAUDIO2_VOICE_STATE aState;
			aEvent->mSourceVoice->GetState(&aState);

			if (aState.BuffersQueued)
			{
				aEvent->mSourceVoice->Stop();
				aEvent->mSourceVoice->FlushSourceBuffers();
			}

			aEvent->mAudioBuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

			if (aEvent)
				return aEvent->start();
		}
		return false;
	}

	bool AudioEngine::pause(std::string name)
	{
		if (mSounds[name])
			return mSounds[name]->pause();

		return false;
	}

	bool AudioEngine::stop(std::string name)
	{
		if (mSounds[name])
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