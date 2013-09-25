bool
noiselang_isnum
(
	lang_processor* prcsr
)
{
	return isdigit(prcsr->current[0]);
}

bool
noiselang_hasval
(
	lang_processor* prcsr
)
{
	return strcmp(prcsr->current, ">") != 0;
}

audio_sound*
noiselang_buildnode
(
	lang_processor* prcsr
)
{
	lang_processor_match(prcsr, "(");

	audio_sound* as;

	if (strcmp(prcsr->current, "time") == 0) {
		lang_processor_match(prcsr, "time");
		as = audio_sound_init(AUDIO_SOUND_TIME, NULL);
	} else
	if (noiselang_isnum(prcsr)) {
		float val = strtod(lang_processor_consume(prcsr), NULL);
		float* valptr = malloc(sizeof(float));
		memcpy(valptr, &val, sizeof(val));
		as = audio_sound_init(AUDIO_SOUND_CONST, (void*)valptr);
	} else
	if (strcmp(prcsr->current, "freq") == 0) {
		lang_processor_match(prcsr, "freq");
		int freq = atoi(lang_processor_consume(prcsr));
		as = audio_sound_init(AUDIO_SOUND_CONST, (void*)&audio_pitch[freq]);
	} else
	if (strcmp(prcsr->current, "sin") == 0) {
		lang_processor_match(prcsr, "sin");
		as = audio_sound_init(AUDIO_SOUND_FUNC, (void*)audio_sound_sin);
		as->args = malloc(sizeof(audio_sound*));
		if (noiselang_hasval(prcsr)) {
			as->args[0] = noiselang_buildnode(prcsr);
		}
	} else
	if (strcmp(prcsr->current, "lfo") == 0) {
		lang_processor_match(prcsr, "lfo");
		as = audio_sound_init(AUDIO_SOUND_FUNC2, (void*)audio_sound_lfo);
		as->args = malloc(sizeof(audio_sound*)*2);
		if (noiselang_hasval(prcsr)) {
			as->args[0] = noiselang_buildnode(prcsr);
		}
		if (noiselang_hasval(prcsr)) {
			as->args[1] = noiselang_buildnode(prcsr);
		}
	} else
	if (strcmp(prcsr->current, "*") == 0) {
		lang_processor_match(prcsr, "*");
		as = audio_sound_init(AUDIO_SOUND_FUNC2, (void*)audio_sound_product);
		as->args = malloc(sizeof(audio_sound*)*2);
		if (noiselang_hasval(prcsr)) {
			as->args[0] = noiselang_buildnode(prcsr);
		}
		if (noiselang_hasval(prcsr)) {
			as->args[1] = noiselang_buildnode(prcsr);
		}
	} else {
		char errorstring[LANG_LINELEN]; 
		sprintf(errorstring, "\"%s\" is not a keyword or identifier!", prcsr->current);
		errorlog_logdef(prcsr->log, "NOISELANG COMPILER", errorstring);
	}
	
	lang_processor_match(prcsr, ")");
	
	return as;
}

audio_sound*
noiselang_buildsound
(
	char** lex,
	errorlog* log
)
{
	lang_processor* prcsr = lang_processor_init(lex, log);
	
	char* name = lang_processor_consume(prcsr);
	
	lang_processor_match(prcsr, "<");
	
	audio_sound* as = noiselang_buildnode(prcsr);

	lang_processor_match(prcsr, ">");

	return as;
}