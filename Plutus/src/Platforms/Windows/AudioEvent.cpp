#include "AudioEvent.h"
#include <Assets/SoundLoader.h>

namespace Plutus
{

	AudioEvent::AudioEvent(const std::string& id, const std::string& filePath) : mSourceVoice(nullptr)
	{
		mName = id;
		mPath = filePath;
		mAudioData.clear();

		bool result = SoundLoader::loadSoundFile(filePath, &mWaveFormat, &mAudioData);
		if (result)
		{
			ZeroMemory(&mAudioBuffer, sizeof(XAUDIO2_BUFFER));
			mAudioBuffer.AudioBytes = (UINT32)mAudioData.size();
			mAudioBuffer.pAudioData = (BYTE* const)&mAudioData[0];
			mAudioBuffer.pContext = nullptr;
		}
	}

	AudioEvent::~AudioEvent()
	{
		mAudioData.clear();
		ZeroMemory(&mAudioBuffer, sizeof(XAUDIO2_BUFFER));

		// delete the source voice
		if (mSourceVoice)
		{
			mSourceVoice->Stop();
			mSourceVoice->DestroyVoice();
		}
	}

	bool AudioEvent::start()
	{
		HRESULT hr = S_OK;

		// submit the audio buffer to the source voice
		hr = mSourceVoice->SubmitSourceBuffer(&mAudioBuffer);
		if (FAILED(hr)) return false;
		// start the source voice
		mSourceVoice->Start();
		state = Playing;
		return true;
	}

	bool AudioEvent::pause()
	{
		mSourceVoice->Stop();
		state = Paused;
		return false;
	}

	bool AudioEvent::stop()
	{
		mAudioBuffer.LoopCount = 0;
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->SubmitSourceBuffer(&mAudioBuffer);
		state = Stopped;
		return true;
	}

	AudioState AudioEvent::getState() {
		XAUDIO2_VOICE_STATE aState;
		mSourceVoice->GetState(&aState);

		return aState.BuffersQueued && state == Playing ? Playing : Stopped;
	}
} // namespace Plutus