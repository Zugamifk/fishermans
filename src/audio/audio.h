#include "fmod.h"

#define AUDIO_ERROR FMOD_RESULT

#define AUDIO_BUFFERSIZE 1024
#define AUDIO_NUMBUFFERS 10

typedef AUDIO_ERROR F_CALLBACK (*audio_readcb)(FMOD_SOUND*, void*, unsigned int);

typedef struct
_S_audiosystem
{
	char* name;
	FMOD_SYSTEM* sys;
	hashtable* sounds;
	set* playing;
} audiosystem;

#include "audio_vals.c"
#include "audio_data.c"
#include "audiobank.c"
#include "audio_sound.c"
#include "audiostream.c"
#include "audiosystem.c"
#include "audio.c"

#include "noiselang/noiselang.h"
