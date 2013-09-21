typedef enum
_E_lang_processor_state
{
	LANG_PROCESSOR_IDLE,
	LANG_PROCESSOR_READING,
	LANG_PROCESSOR_ERROR,
	LANG_PROCESSOR_END
} lang_processor_state;

typedef struct
_S_lang_processor
{
	lang_processor_state state;
	char** stream;
	char** cursor;
	char* current;
	errorlog* log;
	event_bus* bus;
	hashtable* vars;
} lang_processor;

lang_processor*
lang_processor_init
(
	char** stream,
	errorlog* log
)
{
	lang_processor* processor = malloc(sizeof(lang_processor));
	processor->state = LANG_PROCESSOR_READING;
	processor->stream = stream;
	processor->cursor = stream;
	processor->current = *stream;
	processor->log = log;
	return processor;	
}

char*
lang_processor_consume
(
	lang_processor* processor
)
{
	char* res = *(processor->cursor);
	processor->cursor = processor->cursor + 1;
	processor->current = *(processor->cursor);
	return res;
}

int
lang_processor_match
(
	lang_processor* processor,
	char* comp
)
{
	char* res = *(processor->cursor);
	//printf("%s %s\n", res, comp);
	processor->cursor = processor->cursor + 1;
	processor->current = *(processor->cursor);
	int eq = strcmp(res, comp);
	if(eq != 0) {
		char errorstring[LANG_LINELEN]; 
		sprintf(errorstring, "lang processor mismatch error! Expected \'%s\', got \'%s\'", comp, res);
		errorlog_logdef(processor->log, "LANG PROCESSOR", errorstring);
	}
	return eq;
}