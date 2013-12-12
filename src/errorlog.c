#define ERRORLOG_BUFFERLEN 1024
#define	ERRORLOG_MESSAGELEN	1<<10

#define ERRORLOG

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
_errorlog_logerrorlogerror
(
	char*	log,
	char*	error
)
{
	snprintf(log, ERRORLOG_MESSAGELEN, "ERROR LOG ERROR! ERROR CAUSED ERROR! ERROR LOG ERROR:\n\t%s\n", error);
}

void
_errorlog_defaulterror_undecorated
(
	char*	log,
	char**	messages,
	int		messagect
)
{
	char msg[ERRORLOG_MESSAGELEN], frag[ERRORLOG_MESSAGELEN];
	for (int i = 0; i < messagect; i++) {
		sprintf(frag, "%s\n", messages[i]);
		strcat(msg, frag);
	}
	strcpy(log, msg);
}

void
_errorlog_defaulterror
(
	char*	log,
	char**	messages,
	int		messagect
)
{
	if  (messagect > 0) {
		char msg[ERRORLOG_MESSAGELEN], frag[ERRORLOG_MESSAGELEN];
		sprintf(msg, "ERROR: %s\n", messages[0]);
		for (int i = 1; i < messagect; i++) {
			sprintf(frag, "\t%s\n", messages[i]);
			strcat(msg, frag);
		}
		strcpy(log, msg);
	} else {
		snprintf(log, ERRORLOG_MESSAGELEN, "ERROR: -- NO MESSAGE GIVEN --");
	}
}

void
_errorlog_flushbuffer
(
	errorlog*	log
)
{
	for (int i = 0; i < log->bufferlen; i++) {
		fputs(log->buffer[i], log->log);
	}
	log->bufferlen = 0;
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
errorlog_pusherror
(
	errorlog*	log,
	char**		messages,
	int 		messagect
)
{
	/*if (log->bufferlen  == ERRORLOG_BUFFERLEN)*/ _errorlog_flushbuffer(log);
	int place = log->bufferlen;
	log->formaterror(log->buffer[place], messages, messagect); 
	log->bufferlen = log->bufferlen + 1;
}	

void
errorlog_logdef
(
	errorlog*	log,
	char*		errortype,
	char*		errormsg
)
{
	if (errortype == NULL) errortype = "-- NO TYPE GIVEN --";
	if (errormsg == NULL) errormsg = "-- NO ERROR MESSAGE GIVEN --";
	char* msgs[] = {errortype, errormsg};
	errorlog_pusherror(log, msgs, 2);
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

#ifdef EVENT_BUS
typedef struct
_S_errorlog_eventdata
{
	errorlog *log;
	char** messages;
	int messagect;
} errorlog_eventdata;

void
errorlog_errorevent
(
	void*	data
)
{
	errorlog_eventdata* ed = (errorlog_eventdata*)data;
	errorlog_pusherror(ed->log, ed->messages, ed->messagect);
}

#endif