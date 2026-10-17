#include "Player.h"
#include "Vorbis\vorbisfile.h"
#include <tchar.h>

/// <summary>
/// Main Data
/// </summary>
static OALData dataOAL;

/// <summary>
/// OGG STUFF
/// </summary>

#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)

static HINSTANCE g_hVorbisFileDLL = NULL;

// Functions
void InitVorbisFile();
void ShutdownVorbisFile();
unsigned long DecodeOggVorbis(OggVorbis_File* psOggVorbisFile, char* pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels);
void Swap(short& s1, short& s2);

size_t ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource);
int ov_seek_func(void* datasource, ogg_int64_t offset, int whence);
int ov_close_func(void* datasource);
long ov_tell_func(void* datasource);

// Function pointers
typedef int (*LPOVCLEAR)(OggVorbis_File* vf);
typedef long (*LPOVREAD)(OggVorbis_File* vf, char* buffer, int length, int bigendianp, int word, int sgned, int* bitstream);
typedef ogg_int64_t(*LPOVPCMTOTAL)(OggVorbis_File* vf, int i);
typedef vorbis_info* (*LPOVINFO)(OggVorbis_File* vf, int link);
typedef vorbis_comment* (*LPOVCOMMENT)(OggVorbis_File* vf, int link);
typedef int (*LPOVOPENCALLBACKS)(void* datasource, OggVorbis_File* vf, char* initial, long ibytes, ov_callbacks callbacks);

// Variables
LPOVCLEAR			fn_ov_clear = NULL;
LPOVREAD			fn_ov_read = NULL;
LPOVPCMTOTAL		fn_ov_pcm_total = NULL;
LPOVINFO			fn_ov_info = NULL;
LPOVCOMMENT			fn_ov_comment = NULL;
LPOVOPENCALLBACKS	fn_ov_open_callbacks = NULL;

bool g_bVorbisInit = false;

int init() 
{
	ALFWInit();
	
	InitVorbisFile();
	if (!g_bVorbisInit)
	{
		ALFWprintf("Failed to find OggVorbis DLLs (vorbisfile.dll, ogg.dll, or vorbis.dll)\n");
		ALFWShutdown();
		return 0;
	}

	// Initialize Framework
	if (!ALFWInitOpenAL())
	{
		ALFWShutdown();
		return 0;
	}
}

int Play(const char* fileName)
{
	if (dataOAL.filetype == WAV)
	{
		Stop();
		//ALuint      uiBuffer;
		//ALuint      uiSource;
		//ALint       iState;

		// Generate an AL Buffer
		alGenBuffers(1, &dataOAL.uiBuffer);

		// Load Wave file into OpenAL Buffer
		if (!ALFWLoadWaveToBuffer((char*)fileName, dataOAL.uiBuffer))
		{
			ALFWprintf("Failed to load %s\n", fileName);

			return 1;
		}

		// Generate a Source to playback the Buffer
		alGenSources(1, &dataOAL.uiSource);

		// Attach Source to Buffer
		alSourcei(dataOAL.uiSource, AL_BUFFER, dataOAL.uiBuffer);

		alSourcef(dataOAL.uiSource, AL_GAIN, dataOAL.volume);

		// Play Source
		alSourcePlay(dataOAL.uiSource);
	}
	else if (dataOAL.filetype == OGG)
	{
/*		ov_callbacks	sCallbacks;
		OggVorbis_File	sOggVorbisFile;
		vorbis_info* psVorbisInfo;

		sCallbacks.read_func = ov_read_func;
		sCallbacks.seek_func = ov_seek_func;
		sCallbacks.close_func = ov_close_func;
		sCallbacks.tell_func = ov_tell_func;

		// Open the OggVorbis file
		FILE* pOggVorbisFile = fopen(fileName, "rb");
		if (!pOggVorbisFile)
		{
			ALFWprintf("Could not find %s\n", fileName);
			return 0;
		}

		// Create an OggVorbis file stream
		if (fn_ov_open_callbacks(pOggVorbisFile, &sOggVorbisFile, NULL, 0, sCallbacks) == 0)
		{
			// Get some information about the file (Channels, Format, and Frequency)
			psVorbisInfo = fn_ov_info(&sOggVorbisFile, -1);
			if (psVorbisInfo)
			{
				ulFrequency = psVorbisInfo->rate;
				ulChannels = psVorbisInfo->channels;
				if (psVorbisInfo->channels == 1)
				{
					ulFormat = AL_FORMAT_MONO16;
					// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency >> 1;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 2);
				}
				else if (psVorbisInfo->channels == 2)
				{
					ulFormat = AL_FORMAT_STEREO16;
					// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 4);
				}
				else if (psVorbisInfo->channels == 4)
				{
					ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
					// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 2;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 8);
				}
				else if (psVorbisInfo->channels == 6)
				{
					ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
					// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 3;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 12);
				}
			}

			if (ulFormat != 0)
			{
				// Allocate a buffer to be used to store decoded data for all Buffers
				pDecodeBuffer = (char*)malloc(ulBufferSize);
				if (!pDecodeBuffer)
				{
					ALFWprintf("Failed to allocate memory for decoded OggVorbis data\n");
					fn_ov_clear(&sOggVorbisFile);
					ShutdownVorbisFile();
					ALFWShutdownOpenAL();
					ALFWShutdown();
				}

				// Generate some AL Buffers for streaming
				alGenBuffers(NUMBUFFERS, uiBuffers);

				// Generate a Source to playback the Buffers
				alGenSources(1, &uiSource);

				// Fill all the Buffers with decoded audio data from the OggVorbis file
				for (iLoop = 0; iLoop < NUMBUFFERS; iLoop++)
				{
					ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
					if (ulBytesWritten)
					{
						alBufferData(uiBuffers[iLoop], ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
						alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
					}
				}

				// Start playing source
				alSourcePlay(uiSource);

				iTotalBuffersProcessed = 0;
			}
		}*/
	}

	return 0;
}


int Stop()
{
	alSourceStop(dataOAL.uiSource);
	alDeleteSources(1, &dataOAL.uiSource);
	alDeleteBuffers(1, &dataOAL.uiBuffer);

	return 0;
}

int ResumePause()
{
	alGetSourcei(dataOAL.uiSource, AL_SOURCE_STATE, &dataOAL.uiState);
	
	if (dataOAL.uiState == AL_PLAYING)
	{
		alSourcePause(dataOAL.uiSource);
		alGetSourcei(dataOAL.uiSource, AL_SOURCE_STATE, &dataOAL.uiState);
		return 1;
	}
	else
	{
		alSourcePlay(dataOAL.uiSource);
		alGetSourcei(dataOAL.uiSource, AL_SOURCE_STATE, &dataOAL.uiState);
		return 2;
	}
	return -1;
}

int VolumeChange(int _value)
{
	dataOAL.volume = (float)(_value / 100.0f);
	alSourcef(dataOAL.uiSource, AL_GAIN, dataOAL.volume);
	return 1;
}

int GetTotalTime()
{
	ALint size;
	ALint frequency;
	ALint bits;
	ALint channels;

	alGetBufferi(dataOAL.uiBuffer, AL_SIZE, &size);
	alGetBufferi(dataOAL.uiBuffer, AL_FREQUENCY, &frequency);
	alGetBufferi(dataOAL.uiBuffer, AL_BITS, &bits);
	alGetBufferi(dataOAL.uiBuffer, AL_CHANNELS, &channels);

	double durationSeconds = static_cast<double>(size) / (frequency * channels * (bits / 8));

	return durationSeconds;
}

int GetCurrentPlayTime()
{
	ALfloat seconds = 0.0f;

	alGetSourcef(dataOAL.uiSource, AL_SEC_OFFSET, &seconds);

	return static_cast<int>(seconds);
}

void SetCurrentTime(float seconds)
{
	alSourcef(dataOAL.uiSource, AL_SEC_OFFSET, seconds);
}



void SetFileType(std::string string)
{
	string = string.substr(string.find_first_of(".") + 1);
	if (string == "wav")
	{
		dataOAL.filetype = WAV;
	}
	else if (string == "ogg")
	{
		dataOAL.filetype = OGG;
	}
}

///OGG
void InitVorbisFile()
{
	if (g_bVorbisInit)
		return;

	// Try and load Vorbis DLLs (VorbisFile.dll will load ogg.dll and vorbis.dll)
	g_hVorbisFileDLL = LoadLibrary(_T("vorbisfile.dll"));
	if (g_hVorbisFileDLL)
	{
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(g_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(g_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(g_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(g_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(g_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(g_hVorbisFileDLL, "ov_open_callbacks");

		if (fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)
		{
			g_bVorbisInit = true;
		}
	}
}

void ShutdownVorbisFile()
{
	if (g_hVorbisFileDLL)
	{
		FreeLibrary(g_hVorbisFileDLL);
		g_hVorbisFileDLL = NULL;
	}
	g_bVorbisInit = false;
}

unsigned long DecodeOggVorbis(OggVorbis_File* psOggVorbisFile, char* pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels)
{
	int current_section;
	long lDecodeSize;
	unsigned long ulSamples;
	short* pSamples;

	unsigned long ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = fn_ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	if (ulChannels == 6)
	{
		pSamples = (short*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize >> 1); ulSamples += 6)
		{
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			Swap(pSamples[ulSamples + 1], pSamples[ulSamples + 2]);
			Swap(pSamples[ulSamples + 3], pSamples[ulSamples + 5]);
			Swap(pSamples[ulSamples + 4], pSamples[ulSamples + 5]);
		}
	}

	return ulBytesDone;
}

void Swap(short& s1, short& s2)
{
	short sTemp = s1;
	s1 = s2;
	s2 = sTemp;
}

size_t ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void* datasource)
{
	return fclose((FILE*)datasource);
}

long ov_tell_func(void* datasource)
{
	return ftell((FILE*)datasource);
}
