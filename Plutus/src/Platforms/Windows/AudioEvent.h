#pragma once

#include <xaudio2.h>
#include <vector>
#include <string>

namespace Plutus {
	class AudioEngine;

	class AudioEvent {
	private:
		std::string mName;
		std::string mPath;
		//Audio Type
		int type;
		// the XAudio2 source voice
		IXAudio2SourceVoice* mSourceVoice;
		// the length of the wave
		unsigned int mWaveLength;
		// the audio data
		std::vector<uint8_t> mAudioData;
		// the actual buffer with the audio data
		XAUDIO2_BUFFER mAudioBuffer;
		// the format of the audio file
		WAVEFORMATEXTENSIBLE mWaveFormat;

		friend class AudioEngine;
	public:
		AudioEvent(std::string name, std::string filePath);

		~AudioEvent();

		bool start();

		bool stop();

		bool pause();
	};
}