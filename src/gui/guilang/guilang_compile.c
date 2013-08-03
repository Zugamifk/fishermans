void
guilang_compile
(
	errorlog*	log
)
{
	guilang_grammar *spec = guilang_initgrammar("data/guilangspec1", log);
	char *guidata;
	int len;
	load("data/testgui.txt", &guidata, &len);
	
	char** lex = guilang_scan(guidata);
	
	_guilang_token** tokens = guilang_tokenize(lex);
	
	guilang_parse(spec, tokens, log);
}