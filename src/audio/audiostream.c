typedef enum
_E_audio_streamtype
{
	AUDIO_TYPE_NONE = -1,
	AUDIO_BUFFER_PCM16,
	AUDIO_BUFFER_PCMFLOAT,
	AUDIO_FILE_WAV,
	AUDIO_FILE_MP3
} audio_streamtype;

typedef struct
_S_audiostream
{
	FMOD_SOUND* sound;
	signed short* sample;
	int samplelen;
	int samplepos;
	float* spectrum;
	int spectrumlen;
	audio_streamtype type;
} audiostream;

AUDIO_ERROR F_CALLBACK 
audio_testcb
(FMOD_SOUND* s, void* d, unsigned int l)
{
	signed short *cdata = (signed short *)d;
	unsigned int numsamples = l/2;
	
	audiostream* as;
	FMOD_Sound_GetUserData(s, (void*)&as);

	const float freq = audio_data_info->frequencies->range[AUDIO_PITCH_MIDDLEC];
	float volume = 0.3;
	
	for (	int i = 0;
			i < numsamples;
			i+=2	) 
	{
		 int pos = as->samplepos + i/2;
		float t = (float)pos/44100.0 * freq;
		float ft = audio_data_range_get(audio_data_info->square, t);
		cdata[i] = 	 ft * 32767.0 * volume;
		cdata[i+1] = ft * 32767.0 * volume;
	}
	
	as->samplepos = as->samplepos + numsamples/2;
	
	return FMOD_OK;
}

audiostream*
audiostream_init
(
	audiosystem* sys,
	audio_readcb readcb
)
{
	int samplerate = 44100;
	int channels = 2;
	int length = 1; // in seconds
	
	audiostream* as = malloc(sizeof(audiostream));

	as->sound = NULL;
	as->sample = malloc(sizeof(float)*samplerate);
	as->samplelen = samplerate;
	as->samplepos = 0;
	as->spectrum = NULL;
	as->spectrumlen = 0;
	as->type = AUDIO_TYPE_NONE;
	
	FMOD_CREATESOUNDEXINFO fsinfo;
	memset(&fsinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	fsinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	
	fsinfo.decodebuffersize = samplerate; // how much of the buffer to fill per frame/callbacl
	
	int samplesize = sizeof(signed short); // all accepted values are 16-bit right now;
	fsinfo.length = samplerate*channels*samplesize*length;
	
	fsinfo.numchannels = channels;
	fsinfo.defaultfrequency = samplerate;
	
	fsinfo.format = FMOD_SOUND_FORMAT_PCM16;
	fsinfo.pcmreadcallback = readcb;
	
	fsinfo.userdata = as;
	
	FMOD_System_CreateStream(sys->sys, NULL, FMOD_2D | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &fsinfo, &(as->sound));
	
	return as;
}
