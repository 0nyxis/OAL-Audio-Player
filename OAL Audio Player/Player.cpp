#include "Player.h"
#include <al.h>
#include <alc.h>
#include <tchar.h>
#include <vector>

/// <summary>
/// Main Data
/// </summary>
static OALData dataOAL;

/// <summary>
/// OGG STUFF
/// </summary>

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
	
	//InitVorbisFile();
	//if (!g_bVorbisInit)
	//{
	//	ALFWprintf("Failed to find OggVorbis DLLs (vorbisfile.dll, ogg.dll, or vorbis.dll)\n");
	//	ALFWShutdown();
	//	return 0;
	//}

	// Initialize Framework
	if (!ALFWInitOpenAL())
	{
		ALFWShutdown();
		return 0;
	}
}

int Play(const char* fileName)
{
	Stop();

	if (dataOAL.filetype == WAV)
	{
		alGenBuffers(1, &dataOAL.uiBuffer);

		// Load Wave file into OpenAL Buffer
		if (!ALFWLoadWaveToBuffer((char*)fileName, dataOAL.uiBuffer))
		{
			ALFWprintf("Failed to load %s\n", fileName);

			return 0;
		}
	}
	else if (dataOAL.filetype == OGG)
	{
		OggVorbis_File oggFile{};

		if (ov_fopen(fileName, &oggFile) != 0)
		{
			System::Windows::Forms::MessageBox::Show("Error: unable to open file");
			return 0;
		}

		vorbis_info* info = ov_info(&oggFile, -1);

		if (info == nullptr ||
			(info->channels != 1 && info->channels != 2) ||
			info->rate <= 0 ||
			info->rate > (std::numeric_limits<ALsizei>::max)())
		{
			ov_clear(&oggFile);
			System::Windows::Forms::MessageBox::Show("Error: nullptr");
			return 0;
		}

		const ALenum format = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

		const ALsizei sampleRate = static_cast<ALsizei>(info->rate);

		std::vector<char> pcm;
		char chunk[4096];

		int bitstream = 0;
		long bytesRead = 0;

		while ((bytesRead = ov_read(&oggFile, chunk, sizeof(chunk), 0, 2, 1, &bitstream)) > 0)
		{
			if (bytesRead > 0 &&
				pcm.size() + static_cast<size_t>(bytesRead) >
				static_cast<size_t>((std::numeric_limits<ALsizei>::max)()))
			{
				ov_clear(&oggFile);
				System::Windows::Forms::MessageBox::Show("Error: byteRead");
				return 0;
			}

			pcm.insert(pcm.end(), chunk, chunk + bytesRead);
		}

		ov_clear(&oggFile);

		if (bytesRead < 0 || pcm.empty())
		{
			System::Windows::Forms::MessageBox::Show("Error: byteRead2");

			return 0;
		}

		alGenBuffers(1, &dataOAL.uiBuffer);

		alBufferData(dataOAL.uiBuffer,
			format,
			pcm.data(),
			static_cast<ALsizei>(pcm.size()),
			sampleRate);
	}

	alGenSources(1, &dataOAL.uiSource);
	alSourcei(dataOAL.uiSource, AL_BUFFER, dataOAL.uiBuffer);

	alSourcef(dataOAL.uiSource, AL_GAIN, dataOAL.volume);

	alSourcePlay(dataOAL.uiSource);

	return 1;
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