#define ERRCHECK(result) if (result != FMOD_OK){printf("FMOD error! (%s)\n", FMOD_ErrorString(result));exit(-1);}

audio_system *
audio_system_init()
{
	audio_system *as = malloc(sizeof(audio_system));
	
	FMOD_RESULT result;
	
	FMOD_SYSTEM *fs;
	result = FMOD_System_Create(&fs);
	ERRCHECK(result);
	
	as->fmod_system = fs;
	
	result = FMOD_System_Init(fs, 100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	
	audio_tone_init_frequencies();
	
	return as;

}

