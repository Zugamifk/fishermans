gui*
guilang_compile
(
	errorlog*	log,
	event_bus* bus,
	screeninfo* info
)
{
	lang_grammar *spec = lang_initgrammar("data/guilangspec1", log);
	char *guidata;
	int len;
	load("data/testgui.txt", &guidata, &len);
	
	char** lex = lang_scan(guidata);
	
	_lang_token** tokens = lang_tokenize(lex);
	
	lang_parse(spec, tokens, log);
	
	return guilang_buildgui(lex, log, bus, info);
}