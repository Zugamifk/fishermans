#define KEYBOARD_SIZE (1<<CHAR_BIT)
#define KEYBOARD_BUFFERSIZE (1<<12)

typedef enum
_E_keyboard_keytype
{
	KEYBOARD_KEY,
	KEYBOARD_SPECIAL,
	KEYBOARD_WTF
} keyboard_keytype;

typedef struct
keyboard_state_s
{
	char* buffer; //stores recorded input
	unsigned int lastkey; // last pressed key
	unsigned int end; // pointer to end of buffer
	double lasttime; // last time a key was pressed
	double delay; // delay between key presses
	int repeatcount; // how many time a single character has been repeated
	int repeatthreshold; // how many repeats before decrease delay
	double repeatdelay; // delay to change to
	bool validkeys[KEYBOARD_SIZE]; // keys that get recorded
} keyboard_state;

keyboard_state* 
keyboard_state_init
(
	double delay, 
	int threshold,
	double repeatdelay
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

bool
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
		return true;
	} else 
	if (time - state->lasttime > state->delay) {
		state->buffer[state->end] = key;
		state->end = state->end+1;
		state->repeatcount = state->repeatcount + 1;
		return true;
	} return false;
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