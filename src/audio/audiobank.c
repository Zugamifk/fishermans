typedef struct
_S_audiobank
{
	char* name;
	float* data;
	float (*gencb)(int);
	unsigned int len;
} audiobank;

audiobank*
audiobank_init
(
	char* name,
	float (*fun)(int),
	unsigned int len
)
{
	audiobank* ab = malloc(sizeof(audiobank));
	
	ab->name = malloc(strlen(name));
	strcpy(ab->name, name);
	
	ab->data = malloc(sizeof(float)*len);
	memset(ab->data, 0, sizeof(float)*len);
	ab->gencb = fun;
	ab->len = len;
	
	if (fun == NULL) return ab;
	
	for(int i = 0; i < len; i++) {
		ab->data[i] = fun(i);
	}
	
	return ab;
}

