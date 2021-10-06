#include "AudioEvent.h"
#include <Assets/SoundLoader.h>

namespace Plutus
{
	AudioEvent::AudioEvent(std::string name, std::string filePath) : mSourceVoice(nullptr)
	{
		mName = name;
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
		return true;
	}

	bool AudioEvent::pause()
	{
		mSourceVoice->Stop();
		return false;
	}

	bool AudioEvent::stop()
	{
		mAudioBuffer.LoopCount = 0;
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->SubmitSourceBuffer(&mAudioBuffer);
		return true;
	}
} // namespace Plutus