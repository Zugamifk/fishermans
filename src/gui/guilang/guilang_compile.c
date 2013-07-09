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
//	_guilang_tokenpair** tokens = guilang_tokenize(guidata, len);
//	guilang_parse(tokens, spec, log);
}