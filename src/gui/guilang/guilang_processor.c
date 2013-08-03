typedef struct
_S_guilang_processor
{
	char** stream;
	char** cursor;
	char* current;
	errorlog* log;
} guilang_processor;

guilang_processor*
guilang_processor_init
(
	char** stream,
	errorlog* log
)
{
	guilang_processor* processor = malloc(sizeof(guilang_processor));
	processor->stream = stream;
	processor->cursor = stream;
	processor->current = *stream;
	processor->log = log;
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