const int guilang_reqvarnum = 2;
const char* guilang_requiredvars[] = {
	GUIVAR_GUIWIDTH,
	GUIVAR_GUIHEIGHT
};

gui*
guilang_compile
(
	const char* grammarfile,
	const char* guifile,
	errorlog*	log,
	event_bus* bus,
	hashtable* vars
)
{
	lang_grammar *spec = lang_initgrammar(grammarfile, log);
	char *guidata;
	int len;
	load(guifile, &guidata, &len);
	
	char** lex = lang_scan(guidata);
	
	_lang_token** tokens = lang_tokenize(lex);
	
	lang_parse(spec, tokens, log);

	return guilang_buildgui(lex, log, bus, vars);
}

gui*
guilang_load
(
	const char* guifile,
	guilang_version version,
	errorlog*	log,
	event_bus* bus,
	hashtable* vars
)
{
	if (version > GUILANG_LATEST) {
		char errorstring[1024];
		sprintf(errorstring, "Invalid version number %d. Latest version is %d.", version, GUILANG_LATEST);
		errorlog_logdef(log, "GUILANG_LOAD()", errorstring);
		return NULL;
	}
	const char* grammarfile = GUILANG_VERSIONS[version];
	char specpath[1024];
	sprintf(specpath, "%s%s", "data/", grammarfile);
	lang_grammar *spec = lang_initgrammar(specpath, log);
	
	char *guidata;
	int len;
	load(guifile, &guidata, &len);
	
	char** lex = lang_scan(guidata);
	
	_lang_token** tokens = lang_tokenize(lex);
	
	lang_parse(spec, tokens, log);

	return guilang_buildgui(lex, log, bus, vars);
}