#define AUDIO_ERROR_CODE FMOD_RESULT
#define AUDIO_STREAM_TYPE float
#define AUDIO_SAMPLERATE 44100

typedef struct
t_audio_system
{
	FMOD_SYSTEM *fmod_system;
} audio_system;

typedef struct
t_audio_stream
{
	FMOD_SOUND 	*stream;
	float 		(*generatorcb)(float);
	float *		generatorparams;
	int			generatorpramsct;
} audio_stream;

#include "audio_stream.c"
#include "audio_system.c"
#include "audio_test.c"