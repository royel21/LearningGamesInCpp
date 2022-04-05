#pragma once

#include <xaudio2.h>
#include <vector>
#include <string>

namespace Plutus {
	enum AudioState {
		Playing = 1,
		Paused,
		Stopped
	};

	class AudioEngine;

	class AudioEvent {
	private:
		std::string mName;
		std::string mPath;
		AudioState state = Stopped;
		//Audio Type
		int type;
		// the length of the wave
		unsigned int mWaveLength;
		// the actual buffer with the audio data
		XAUDIO2_BUFFER mAudioBuffer;
		// the format of the audio file
		WAVEFORMATEXTENSIBLE mWaveFormat;
		// the XAudio2 source voice
		IXAudio2SourceVoice* mSourceVoice;
		// the audio data
		std::vector<uint8_t> mAudioData;

		friend class AudioEngine;
	public:
		AudioEvent(const std::string& id, const std::string& filePath);

		~AudioEvent();

		bool start();

		bool stop();

		bool pause();

		AudioState getState();
		std::string getPath() { return mPath; }
	};
}