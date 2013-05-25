#define AUDIO_ERROR_CODE FMOD_RESULT
#define AUDIO_STREAM_TYPE unsigned int

typedef struct
t_audio_system
{
	FMOD_SYSTEM *fmod_system;
} audio_system;

#include "audio_system.c"