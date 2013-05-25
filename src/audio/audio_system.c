#define ERRCHECK(result) if (result != FMOD_OK){printf("FMOD error! (%s)\n", FMOD_ErrorString(result));exit(-1);}

AUDIO_STREAM_TYPE
_audio_tonegentest
(float n)
{
	return sin(2.0*pi* n/500.0)
		* (1<<14);
}

AUDIO_ERROR_CODE
F_CALLBACK
_audio_system_bufferreadcb
(
	FMOD_SOUND *sound, 
	BYTE *data,
	unsigned int len
)
{
	AUDIO_STREAM_TYPE *cdata = (AUDIO_STREAM_TYPE *)data;
	unsigned int numsamples = len/4;
	
	for (	int i = 0;
			i < numsamples;
			i+=2	) 
	{
		AUDIO_STREAM_TYPE val = _audio_tonegentest((float)i);
		cdata[i] = 	val;
		cdata[i+1] = val;
	}
	for (	int i = 0;
			i < numsamples;
			i+=1	)
	{
//printf("%d\n", cdata[i]);
	}
	return FMOD_OK;
}

audio_system *
audio_system_init()
{

	audio_system *as = malloc(sizeof(audio_system));
	
	FMOD_RESULT result;
	
	FMOD_SYSTEM *fs = as->fmod_system;
	result = FMOD_System_Create(&fs);
	ERRCHECK(result);
	
	result = FMOD_System_Init(fs, 100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	
	FMOD_SOUND *sound;
	const char *data = malloc(sizeof(int)*(1<<16));
	// for (int i=0; i< 1<<16; i++) {
		// float t = (float)i;
		// data[i] = 0;
	// }
	
	FMOD_CREATESOUNDEXINFO exinfo;

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

	exinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
	exinfo.decodebuffersize  = 44100;                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	exinfo.length            = 44100 * 2 * sizeof(AUDIO_STREAM_TYPE) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
	exinfo.numchannels       = 2;                                    /* Number of channels in the sound. */
	exinfo.defaultfrequency  = 44100;                                       /* Default playback rate of sound. */
	exinfo.format            = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
	exinfo.pcmreadcallback   = _audio_system_bufferreadcb;                             /* User callback for reading. */
	// exinfo.pcmsetposcallback = pcmsetposcallback;                           /* User callback for seeking. */

	result = FMOD_System_CreateStream(fs, NULL, FMOD_2D | FMOD_OPENUSER, &exinfo, &sound);
	ERRCHECK(result);
	
		printf("?\n");
	
	FMOD_CHANNEL *channel;
	result = FMOD_System_PlaySound(fs, FMOD_CHANNEL_FREE, sound, 0, &channel);
	ERRCHECK(result);
	
	return as;

}

