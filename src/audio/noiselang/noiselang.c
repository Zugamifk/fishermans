lang_grammar*
noiselang_init
(
	const char* grammarfile,
	errorlog*	log
)
{
	return lang_initgrammar(grammarfile, log);
}

audio_sound*
noiselang_loadsound
(
	const char* soundfile,
	lang_grammar* grammar,
	errorlog* log
)
{
	char *data;
	int len;
	load(soundfile, &data, &len);
	
	char** lex = lang_scan(data);
	
	_lang_token** tokens = lang_tokenize(lex);
	
	lang_parse(grammar, tokens, log);

	return noiselang_buildsound(lex, log);
}
