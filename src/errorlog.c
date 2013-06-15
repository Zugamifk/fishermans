#define ERRORLOG_BUFFERLEN 1024
#define	ERRORLOG_MESSAGELEN	1024

typedef void (*_cb_errorlog_errorformat)(char*, char**, int);

typedef struct
_S_errorlog
{
	char* name;
	FILE *log;
	char **buffer;
	unsigned int bufferlen;
	_cb_errorlog_errorformat formaterror;
} errorlog;

void
_errorlog_defaulterror
(
	char*	log,
	char**	messages,
	int		messagect
)
{
	if (messagect != 3) return;
	snprintf(log, ERRORLOG_MESSAGELEN, "ERROR! <%s>:\n\t %s\n\t\t%s\n", messages[0], messages[1], messages[2]);
}

void
_errorlog_flushbuffer
(
	errorlog*	log
)
{
	for (int i = 0; i < log->bufferlen; i--) {
		int success = fputs(log->buffer[i], log->log);
		printf("%d\n", success);
	}
	log->bufferlen = 0;
}

void
errorlog_pusherror
(
	errorlog*	log,
	char**		messages,
	int 		messagect
)
{
	if (log->bufferlen  == ERRORLOG_BUFFERLEN) _errorlog_flushbuffer(log);
	int place = log->bufferlen;
	log->formaterror(log->buffer[place], messages, messagect); 
	log->bufferlen++;
	printf("%s", log->buffer[place]);
}	

errorlog*
errorlog_init
(
	char*	name,
	char*	filename,
	unsigned int append
)
{
	errorlog* elog = malloc(sizeof(errorlog));

	elog->name = name;
	elog->log = fopen(filename, append?"a":"w");
	
	if (elog->log==NULL) {
		printf("FILE ERROR!\n");
		return NULL;
	}
	
	elog->buffer = malloc(sizeof(char*)*ERRORLOG_BUFFERLEN);
	for (int i = ERRORLOG_BUFFERLEN; i--;) {
		elog->buffer[i] = malloc(sizeof(char)*ERRORLOG_MESSAGELEN);
	}
	
	elog->bufferlen = 0;
	elog->formaterror = _errorlog_defaulterror;
	
	return elog;
}

void
errorlog_logdef
(
	errorlog*	log,
	char*		errortype,
	char*		errormsg
)
{
	
	char* msgs[] = {log->name, errortype, errormsg};
	errorlog_pusherror(log, msgs, 3);
}

void
errorlog_free
(
	errorlog*	log
)
{
	_errorlog_flushbuffer(log);
	
	free(log->name);
	fclose(log->log);
	for(int i = ERRORLOG_BUFFERLEN; i--;) {
		free(log->buffer[i]);
	}
	free(log->buffer);
	free(log);
}