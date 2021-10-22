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

		bool add(const std::string& name, std::string path, SoundType type);
		void remove(const std::string& name);

		bool playEvent(AudioEvent* tempEvent, bool loop = false);
		bool play(const std::string& name, bool loop = false);
		bool pauseEvent(AudioEvent* tempEvent);
		bool stopEvent(AudioEvent* tempEvent);

		bool pause(const std::string& name);
		bool stop(const std::string& name);

		void cleanUp();
		AudioEvent* createEvent(const std::string& name, const std::string& path, SoundType type);
		AudioMap& getItems() { return mSounds; }

	private:
		static bool has_COINIT;
		AudioEngine();
		AudioEngine(AudioEngine& ae) = delete;
		bool Init();
	};

	extern AudioEngine& SoundEngine;
} // namespace Plutus