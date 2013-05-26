audio_stream *_audio_test_stream;
AUDIO_STREAM_TYPE (*audio_generator)(float);
float **audio_vars;
float audio_time;

AUDIO_STREAM_TYPE
_audio_tonegentest
(float n)
{

	float samplerate = AUDIO_SAMPLERATE;
	float cycle = 2.0*pi; 
	float pos = n*cycle;
	
	float pulseamplitude = *(audio_vars[1]);
	float pulse = 1.0+cos(pos*pulseamplitude);
	
	float lowkey = 10;
	float highkey = 78;
	float key = *(audio_vars[2]) * (highkey - lowkey) + lowkey;
	float pitch = pow(2.0, (key - 49.0)/12.0)*440.0;
	
	//float drop = 1.0/(10.0*(*(audio_vars[3]))*n+0.0001);
	
	float tone = sin(pos*pitch)*pulse;
	
	return tone;
}


AUDIO_ERROR_CODE
F_CALLBACK
_audio_stream_bufferreadcb
(
	FMOD_SOUND *sound, 
	void *data,
	unsigned int len
)
{
	AUDIO_STREAM_TYPE *cdata = (AUDIO_STREAM_TYPE *)data;
	unsigned int numsamples = len/4;
	
	for (	int i = 0;
			i < numsamples;
			i+=2	) 
	{
		AUDIO_STREAM_TYPE val = audio_generator(audio_time);
		cdata[i] = 	val;
		cdata[i+1] = val;
		audio_time+=1.0/(float)AUDIO_SAMPLERATE;
	//	printf("%f\n", audio_time);
	}

	return FMOD_OK;
}

void
audio_test_init
(
	audio_system 	*as,
	float			*cbparams,
	int				paramsct
)
{
	audio_generator = _audio_tonegentest;
	audio_time = 0;
	audio_vars = malloc(sizeof(float *)*(1+paramsct));
	audio_vars[0] = &audio_time;
	for(int i=0;i<paramsct;i++) {
		audio_vars[i+1] = cbparams+i;
	}
	
	_audio_test_stream = audio_stream_init(as, _audio_stream_bufferreadcb);
	_audio_test_stream->generatorparams 	= cbparams;
	_audio_test_stream->generatorpramsct	= paramsct;
	
	audio_stream_play(as, _audio_test_stream);
}
