#pragma once
#include "Framework.h"
#include <vorbis/vorbisfile.h>

enum FileType
{
	WAV,
	OGG,
};

struct OALData
{
	ALuint      uiBuffer;
	ALuint      uiSource;
	ALint       uiState;

	FileType filetype = FileType::WAV;

	float       volume = 1.0f;
};

int Play(const char* fileName);
int Stop();
int init();
int ResumePause();

int GetTotalTime();
int GetCurrentPlayTime();
void SetCurrentTime(float _seconds);

int VolumeChange(int _value);
void SetFileType(std::string string);