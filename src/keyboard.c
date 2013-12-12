#define KEYBOARD_SIZE (1<<CHAR_BIT)
#define KEYBOARD_BUFFERSIZE (1<<12)

typedef struct
keyboard_state_s
{
	char* buffer;
	unsigned int end;
	double lasttime;
	double delay;
	int repeatcount;
	int repeatthreshold;
	double repeatdelay;
	bool validkeys[KEYBOARD_SIZE];
} keyboard_state;

keyboard_state* 
keyboard_state_init
(
	double delay, 
	int threshold,
	int repeatdelay
)
{
	keyboard_state* state = malloc(sizeof(keyboard_state));
	state->buffer = calloc(1, KEYBOARD_BUFFERSIZE);
	state->end = 0;
	state->lasttime = 0.0;
	state->delay = delay;
	state->repeatcount = 0;
	state -> repeatthreshold = threshold;
	state->repeatdelay = repeatdelay;
	for (int i=0; i<KEYBOARD_SIZE;i++) {
		state->validkeys[i] = true;
	}
	return state;
}

void
keyboard_state_update
(
	keyboard_state* state,
	char key,
	double time
)
{
	if (state->repeatcount >= state->repeatthreshold && key == state->buffer[state->end-1]) {
		if (time - state->lasttime > state->repeatdelay) {
			state->buffer[state->end] = key;
			state->end = state->end+1;
		} 
		return;
	} else 
	if (time - state->lasttime > state->delay) {
		state->buffer[state->end] = key;
		state->end = state->end+1;
		state->repeatcount = state->repeatcount + 1;
	}
}

void
keyboard_state_flushbuffer
(
	keyboard_state* state
)
{
	for (int i = 0; i<state->end;i++) {
		state->buffer[i] = 0;
	}
	state->end = 0;
	state->repeatcount = 0;
}

void
keyboard_state_getbuffer
(
	char* dest,
	keyboard_state* state
)
{
	strcpy(dest, state->buffer);
	keyboard_state_flushbuffer(state);
}