audio_stream *_audio_test_stream;
AUDIO_STREAM_TYPE (*audio_generator)(float);
float **audio_vars;
float audio_time;

AUDIO_STREAM_TYPE
_audio_tone
(float n)
{
	float tone = 0;
	
	float cycle = 2.0*pi; 
	float pos = n*cycle;
	
	float samplelen = cycle;
	float sustain = *(audio_vars[1]);
	
	int range = 3**(audio_vars[4]);
	int lowkey = 4+12*range;
	int highkey = 87-12*range;
	int key = *(audio_vars[7]) * (highkey - lowkey) + lowkey;
	float noise = *(audio_vars[5]);
	
	float noisepeak = *(audio_vars[6])*samplelen;
	float tonepeak = *(audio_vars[3])*samplelen;
	
	float noiseadsr = audio_tone_adsr(pos, samplelen, noisepeak*0.9, noisepeak*0.95, sustain, samplelen-0.2);
	float toneadsr = audio_tone_adsr(pos, samplelen, tonepeak*0.9, tonepeak*0.95, sustain, samplelen-0.2);
	
	float d = 1;
	for (int i = -range; i <= range; i++) {
		tone += toneadsr*audio_tone_sin(key+i*12, pos, i*cycle)*(1.0-noise)/d;
		d+=1;
	}	
	
	tone += noiseadsr*audio_tone_noise()*noise;
	
	tone += math_usin(n)*audio_tone_square(16, pos, 0)*0.1**(audio_vars[2]);
	
	return tone;
}

AUDIO_STREAM_TYPE
_audio_tonegentest
(float n)
{
	float signal = _audio_tone(n);
	
	return signal;

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
		*(audio_vars+i+1) = cbparams+i;
	}
	
	_audio_test_stream = audio_stream_init(as, _audio_stream_bufferreadcb);
	_audio_test_stream->generatorparams 	= cbparams;
	_audio_test_stream->generatorpramsct	= paramsct;
	
	audio_stream_play(as, _audio_test_stream);
}
