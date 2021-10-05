#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <Utils/types.h>


namespace Plutus {

	struct SubFormat {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};

	//Copied from microsoft WAVEFORMATEX
	struct WaveFormatEx {
		u16        wFormatTag;         /* format type */
		u16        nChannels;          /* number of channels (i.e. mono, stereo...) */
		ulong       nSamplesPerSec;     /* sample rate */
		ulong       nAvgBytesPerSec;    /* for buffer estimation */
		u16        nBlockAlign;        /* block size of data */
		u16        wBitsPerSample;     /* number of bits per sample of mono data */
		u16        cbSize;             /* the count in bytes of the size of extra information (after cbSize) */
		union {
			u16 wValidBitsPerSample;       /* bits of precision  */
			u16 wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
			u16 wReserved;                 /* If neither applies, set to zero. */
		} Samples;
		ulong           dwChannelMask;      /* which channels are */
		SubFormat subformat;
	};

	class SoundLoader
	{
	public:
		static bool loadSoundFile(const std::string& filePath, void* wfx, std::vector<u8>* buffer);

	private:
		static bool readChunkToBuffer(std::string file, void* buffer, unsigned long buffSize, unsigned long offset);
		static bool readWave(std::string filePath, void* wfx, unsigned long& buffSize, unsigned long& offset);
		static bool readOgg(std::string filePath, void* wfx, std::vector<u8>* buffer);
	};
}