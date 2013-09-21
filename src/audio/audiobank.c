typedef struct
_S_audiobank
{
	float min;
	float max;
	float interval;
	unsigned int len;
	float* data;
} audiobank;

audiobank*
audiobank_init
(
	float* data,
	float min,
	float max,
	unsigned int len
)
{
	audiobank* ab = malloc(sizeof(audiobank));

	ab->min = min;
	ab->max = max;
	ab->interval = max - min;
	ab->len = len;
	ab->data = data;
	
	return ab;
}

float
audiobank_get
(
	audiobank* bank,
	float x
)
{
	x = fmod(x-bank->min, bank->interval);
	int i = x/bank->interval * bank->len;
	return bank->data[i];
}

void
audiobank_free
(
	audiobank* bank
)
{
	free(bank->data);
	free(bank);
}
