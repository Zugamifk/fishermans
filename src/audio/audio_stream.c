audio_stream *
audio_stream_init
(
	audio_system *a,
	AUDIO_ERROR_CODE F_CALLBACK (*gencb)(FMOD_SOUND *, void *, unsigned int)
)
{
	
	audio_stream *newstream = malloc(sizeof(audio_stream));
		
	FMOD_CREATESOUNDEXINFO exinfo;

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

	exinfo.cbsize            = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.decodebuffersize  = AUDIO_SAMPLERATE;
	exinfo.length            = AUDIO_SAMPLERATE * 2 * sizeof(AUDIO_STREAM_TYPE) * 5;
	exinfo.numchannels       = 2;
	exinfo.defaultfrequency  = AUDIO_SAMPLERATE;
	exinfo.format            = FMOD_SOUND_FORMAT_PCMFLOAT;  
	exinfo.pcmreadcallback   = gencb; 

	FMOD_System_CreateStream(a->fmod_system, NULL, FMOD_2D | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &(newstream->stream));
	
	return newstream;
}

FMOD_RESULT
audio_stream_play
(audio_system *a, audio_stream *s)
{
	FMOD_RESULT result;
	FMOD_CHANNEL *channel;
	result = FMOD_System_PlaySound(a->fmod_system, FMOD_CHANNEL_FREE, s->stream, 0, &channel);
	result |= FMOD_Channel_SetLoopCount(channel, -1);
	return result;
}