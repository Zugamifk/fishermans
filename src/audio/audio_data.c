#define AUDIO_DATA_ARRLEN 1024
#define AUDIO_DATA_ARRLENF 1024.0

typedef struct
_S_audio_data_rangef
{
	float min;
	float max;
	float* range;
} audio_data_rangef;
typedef float (*_audio_rangeffun)(float);

audio_data_rangef*
audio_data_rangef_init
(
	float (*fun)(float),
	float min,
	float max
)
{
	if (min >= max) return NULL;
	
	audio_data_rangef* range = malloc(sizeof(audio_data_rangef));
	
	range->min = min;
	range->max = max;
	range->range = malloc(sizeof(float)*AUDIO_DATA_ARRLEN);
	
	float step = (max - min) / AUDIO_DATA_ARRLENF;
	float val = min;
	
	for (int i = 0; i < AUDIO_DATA_ARRLEN; i++) {
		range->range[i] = fun(val);
		val = val + step;
	}
	
	return range;
}

float
audio_data_range_get
(
	const audio_data_rangef* r,
	float f
)
{
	float interval = r->max - r->min;
	int i = fmod(f, interval)/interval * AUDIO_DATA_ARRLENF;
	return r->range[i];
}

typedef struct
_S_audio_data_rangei
{
	float* range;
	int len;
} audio_data_rangei;

typedef enum
_E_audio_data_rangei_types {
	AUDIO_DATA_RANGEI_FUN,
	AUDIO_DATA_RANGEI_ARR
} audio_data_rangei_types;

audio_data_rangei*
audio_data_rangei_init
(
	audio_data_rangei_types type,
	int len,
	void* data
)
{
	audio_data_rangei* range = malloc(sizeof(audio_data_rangei));
	
	range->range = malloc(sizeof(float)*len);
	
	switch (type) {
		case AUDIO_DATA_RANGEI_FUN: {
			float (*fun)(int) = (float (*)(int))data;
			for (int i = 0; i < len; i++) {
				range->range[i] = fun(i);
			}
		} break;
		case AUDIO_DATA_RANGEI_ARR: {
			float* arr = (float*)data;
			for (int i = 0; i < len; i++) {
				range->range[i] = arr[i];
			}
		} break;
	}

	range->len = len;
	
	return range;
}

typedef struct
_S_audio_data
{
	const audio_data_rangei* frequencies;
	const audio_data_rangef* sin;
	const audio_data_rangef* cos;
	const audio_data_rangef* tan;
	const audio_data_rangef* ln;
	const audio_data_rangef* square;
	const audio_data_rangef* saw;
} audio_data;

bool audio_data_initialized;
audio_data* audio_data_info;

float _audio_data_sin(float x) {return sin(x);}
float _audio_data_cos(float x) {return cos(x);}
float _audio_data_square(float x) {return x<0.5?-1.0:1.0;}
float _audio_data_saw(float x) {return 2.0*x-1.0;}

audio_data*
audio_data_init()
{
	if (audio_data_initialized) return audio_data_info;

	audio_data* data = malloc(sizeof(audio_data));
	
	data->frequencies = audio_data_rangei_init(AUDIO_DATA_RANGEI_ARR, audio_pitch_ct, (void*)audio_pitch);
	data->sin = audio_data_rangef_init(_audio_data_sin, -pi, pi);
	data->cos = audio_data_rangef_init(_audio_data_cos, -pi, pi);
	data->square = audio_data_rangef_init(_audio_data_square, 0.0, 1.0);
	data->saw = audio_data_rangef_init(_audio_data_saw, 0.0, 1.0);
	
	audio_data_initialized = true;
	audio_data_info = data;
	return data;
}

audio_data*
audio_data_initdb()
{
	audio_data* ad = audio_data_init();
	
	printf("FREQUENCIES:\n[\t");
	for (int i = 0; i < ad->frequencies->len && printf(","); i++) {
		printf("%d: %f", i, ad->frequencies->range[i]);
	}
	printf("]\n");
	
	printf("SIN (-pi, pi):\n[\t");
	for (int i = 0; i < AUDIO_DATA_ARRLEN; i++, printf(",")) {
		printf("%d: %f", i, ad->sin->range[i]);
	}
	printf("]\n");
	
	return ad;
}