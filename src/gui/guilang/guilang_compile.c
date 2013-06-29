void
guilang_compile
(
	errorlog*	log
)
{
	guilang_specification *spec = guilang_initspecification("guilang spec.txt");
	char *guidata;
	int len;
	load("testgui.txt", &guidata, &len);
	_guilang_tokenpair** tokens = guilang_tokenize(guidata, len);
	guilang_parse(tokens, spec, log);
}