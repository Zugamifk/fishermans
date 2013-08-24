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