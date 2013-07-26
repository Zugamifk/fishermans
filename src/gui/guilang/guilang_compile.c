void
guilang_compile
(
	errorlog*	log
)
{
	guilang_grammar *spec = guilang_initgrammar("guilang spec.txt", log);
	char *guidata;
	int len;
	load("testgui.txt", &guidata, &len);
	
	char** lex = guilang_scan(guidata);
	
	_guilang_token** tokens = guilang_tokenize(lex);
	
//	guilang_parse(tokens, spec, log);
}