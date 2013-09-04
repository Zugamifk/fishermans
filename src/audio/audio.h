#include "portaudio.h"

typedef struct
_S_audiostream
{
	PaStream* _pa_stream;
} audiostream;