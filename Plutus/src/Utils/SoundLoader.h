#pragma once

#include <vector>
#include <string>
#include <stdint.h>

namespace Plutus {

	struct SubFormat {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};

	//Copied from microsoft WAVEFORMATEX
	struct WaveFormatEx {
		uint16_t        wFormatTag;         /* format type */
		uint16_t        nChannels;          /* number of channels (i.e. mono, stereo...) */
		uint32_t       nSamplesPerSec;     /* sample rate */
		uint32_t       nAvgBytesPerSec;    /* for buffer estimation */
		uint16_t        nBlockAlign;        /* block size of data */
		uint16_t        wBitsPerSample;     /* number of bits per sample of mono data */
		uint16_t        cbSize;             /* the count in bytes of the size of extra information (after cbSize) */
		union {
			uint16_t wValidBitsPerSample;       /* bits of precision  */
			uint16_t wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
			uint16_t wReserved;                 /* If neither applies, set to zero. */
		} Samples;
		uint32_t           dwChannelMask;      /* which channels are */
		SubFormat subformat;
	};

	class SoundLoader
	{
	public:
		static bool loadSoundFile(const std::string& filePath, void* wfx, std::vector<uint8_t>* buffer);

	private:
		static bool readChunkToBuffer(std::string file, void* buffer, unsigned long buffSize, unsigned long offset);
		static bool readWave(std::string filePath, void* wfx, unsigned long& buffSize, unsigned long& offset);
		static bool readOgg(std::string filePath, void* wfx, std::vector<uint8_t>* buffer);
	};
}