typedef enum
_E_guilang_processor_state
{
	GUILANG_PROCESSOR_IDLE,
	GUILANG_PROCESSOR_READING,
	GUILANG_PROCESSOR_ERROR,
	GUILANG_PROCESSOR_END
} guilang_processor_state;

typedef struct
_S_guilang_processor
{
	guilang_processor_state state;
	char** stream;
	char** cursor;
	char* current;
	errorlog* log;
	event_bus* bus;
} guilang_processor;

guilang_processor*
guilang_processor_init
(
	char** stream,
	errorlog* log, 
	event_bus* bus
)
{
	guilang_processor* processor = malloc(sizeof(guilang_processor));
	processor->state = GUILANG_PROCESSOR_READING;
	processor->stream = stream;
	processor->cursor = stream;
	processor->current = *stream;
	processor->log = log;
	processor->bus = bus;
	return processor;	
}

char*
guilang_processor_consume
(
	guilang_processor* processor
)
{
	char* res = *(processor->cursor);
	processor->cursor = processor->cursor + 1;
	processor->current = *(processor->cursor);
	return res;
}

int
guilang_processor_match
(
	guilang_processor* processor,
	char* comp
)
{
	char* res = *(processor->cursor);
	processor->cursor = processor->cursor + 1;
	processor->current = *(processor->cursor);
	int eq = strcmp(res, comp);
	if(eq != 0) {
		char errorstring[LANG_LINELEN]; 
		sprintf(errorstring, "guilang processor mismatch error! Expected \'%s\', got \'%s\'", comp, res);
		errorlog_logdef(processor->log, "GUILANG PROCESSOR", errorstring);
	}
	return eq;
}