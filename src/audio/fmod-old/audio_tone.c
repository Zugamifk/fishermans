void
audio_tone_init_frequencies
(
	void
)
{
	audio_tone_frequencies = malloc(sizeof(float)*AUDIO_TONE_FRQCT);
	for (int i = 0; i < AUDIO_TONE_FRQCT; i++)
	{
		audio_tone_frequencies[i] = pow(2.0, (i - 49.0)/12.0)*440.0;
	}
}

float
audio_tone_sin
(
	int key,
	float t,
	float p
)
{
	return sin(audio_tone_frequencies[key]*t+p);
}

float
audio_tone_square
(
	int key,
	float t,
	float p
)
{
	return sin(audio_tone_frequencies[key]*t+p)>0?1:-1;
}

float
audio_tone_saw
(
	int key,
	float t,
	float p
)
{
	return fmod(audio_tone_frequencies[key]*t+p, 2*pi)/pi-1;
}

float
audio_tone_noise
()
{
	return 1.0-(rand()%1000)/500.0;
}

float
audio_tone_adsr
(
	float t,
	float p,
	float attack,
	float delay,
	float sustain,
	float decay
)
{
	float dt = fmod(t, p);
	if (dt < attack)
		return dt/attack;
		
	if (dt < delay) 
		return (1.0-(dt - attack)/(delay - attack))*(1.0 - sustain)+sustain;
	
	if (dt < decay)
		return sustain;
	
	return (1.0-(dt - decay)/(p-decay))*sustain;
}