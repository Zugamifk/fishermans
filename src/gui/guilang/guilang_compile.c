void
guilang_compile
(
	errorlog*	log
)
{
	guilang_grammar *spec = guilang_initgrammar("testspec.txt", log);
	char *guidata;
	int len;
	load("testgui2.txt", &guidata, &len);
	
	char** lex = guilang_scan(guidata);
	
	_guilang_token** tokens = guilang_tokenize(lex);
	
	guilang_parse(spec, tokens);
}