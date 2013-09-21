typedef enum
_E_audio_sound_valuetype
{
	AUDIO_SOUND_TIME,
	AUDIO_SOUND_CONST,
	AUDIO_SOUND_FUNC,
	AUDIO_SOUND_FUNC2,
	AUDIO_SOUND_BANK
} audio_sound_valuetype;

typedef union
_U_audio_sound_value
{
	audiobank* bank;
	float (*fun)(float);
	float (*fun2)(float, float);
	float constant;
} audio_sound_value;

typedef struct
_S_audio_sound
{
	struct _S_audio_sound** args;
	audio_sound_valuetype type;
	audio_sound_value val;
} audio_sound;


float audio_sound_sin(float x) {return sin(x*2.0*pi);}
float audio_sound_cos(float x) {return cos(x);}
float audio_sound_square(float x) {return x<0.5?-1.0:1.0;}
float audio_sound_saw(float x) {return 2.0*x-1.0;}
float audio_sound_product(float x, float y) {return x*y;}

audio_sound*
audio_sound_init
(
	audio_sound_valuetype type,
	void* value
)
{
	audio_sound* sound = malloc(sizeof(audio_sound));
	
	sound->type = type;
	
	sound->args = NULL;
	
	switch(type) {
		case AUDIO_SOUND_TIME: break;
		case AUDIO_SOUND_CONST: sound->val.constant = *(float*)value; break;
		case AUDIO_SOUND_FUNC: {
			sound->val.fun = value; 
		} break;
		case AUDIO_SOUND_FUNC2: {
			sound->val.fun2 = value; 
		} break;
		case AUDIO_SOUND_BANK: {
			sound->val.bank = NULL;
		} break;
	}
	
	return sound;
}

float
audio_sound_get
(
	audio_sound* as,
	float t
)
{
	float res = 0.0;
	switch (as->type) {
		case AUDIO_SOUND_TIME: res = t; break;
		case AUDIO_SOUND_CONST: res = as->val.constant; break;
		case AUDIO_SOUND_FUNC: {
			audio_sound* arg = as->args[0];
			float x = audio_sound_get(arg, t);
			res = as->val.fun(x);
		} break;
		case AUDIO_SOUND_FUNC2: {
			float x = audio_sound_get(as->args[0], t);
			float y = audio_sound_get(as->args[1], t);
			res = as->val.fun2(x, y);
		} break;
		case AUDIO_SOUND_BANK: {
			//float x = audio_sound_get(as->args[0], t);
			res = audiobank_get(as->val.bank, t);
		} break;
	}
	return res;
}

float*
audio_sound_eval
(
	audio_sound* as,
	float len,
	float samplerate,
	float t
)
{
	int alen = len*samplerate;
	float* data = malloc(sizeof(float)*alen);
	
	float inc = 1.0/samplerate;
	for(int i = 0; i<alen;i++) {
		data[i] = audio_sound_get(as, t);
		t = t + inc;
	}
	
	return data;
}

void
audio_sound_bank
(
	audio_sound* as,
	float len,
	float samplerate
)
{
	switch (as->type) {
		case AUDIO_SOUND_TIME:
		case AUDIO_SOUND_CONST: return;
		default: break;
	}
	
	float* data = audio_sound_eval(as, len, samplerate, 0.0);
	as->type = AUDIO_SOUND_BANK;
	
	unsigned int dlen = len*samplerate;
	as->val.bank = audiobank_init(data, 0.0, len, dlen);
}

void
audio_sound_print
(
	audio_sound* as
)
{
	printf ("SOUND:\n");
	printf("DATA: [");
	float* data = audio_sound_eval(as, 1.0, 100.0, 0.0);
	for (int i = 0; i<100;i++) {
		printf ("%f, ",data[i]);
	}
	printf(" ]\n");
}
