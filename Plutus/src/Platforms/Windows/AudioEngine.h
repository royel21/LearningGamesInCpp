#pragma once

#include <string>
#include <xaudio2.h>
#include <unordered_map>

#pragma comment(lib, "xaudio2.lib")

namespace Plutus
{

	class AudioEvent;

	using AudioMap = std::unordered_map<std::string, AudioEvent*>;

	enum SoundType
	{
		MUSIC,
		EFFECT
	};

	class AudioEngine
	{
	private:
		IXAudio2* mXAuido2;
		IXAudio2MasteringVoice* mMasterVoice;
		// submix voices
		IXAudio2SubmixVoice* mSoundsSubmix; // collection of all sound effects
		IXAudio2SubmixVoice* mMusicSubmix;	// collection of all music files

		XAUDIO2_SEND_DESCRIPTOR sendSounds;

		XAUDIO2_SEND_DESCRIPTOR sendMusic;

		XAUDIO2_VOICE_SENDS mSoundsSendList;
		XAUDIO2_VOICE_SENDS mMusicSendList;

		AudioMap mSounds;

	public:
		static AudioEngine& get();
		~AudioEngine();

		bool add(std::string name, std::string path, SoundType type);
		void remove(std::string name);

		bool play(std::string name, bool loop = false);
		bool pause(std::string name);
		bool stop(std::string name);

		void cleanUp();

		AudioMap& getItems() { return mSounds; }

	private:
		static bool has_COINIT;
		AudioEngine();
		AudioEngine(AudioEngine& ae) = delete;
		bool Init();
	};

	extern AudioEngine& SoundEngine;
} // namespace Plutus