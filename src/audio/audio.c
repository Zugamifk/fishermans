typedef struct
_S_audiostream
{
	FMOD_SOUND* data;
	int len;
	float* spectrum;
	int spectrumlen;
} audiostream;

audiostream*
audiostream_init
(

)
{
	audiostream* as = malloc(sizeof(audiostream));
//	PaError err;
	
	
	return as;
}