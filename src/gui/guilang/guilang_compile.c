void
guilang_compile
(

)
{
//	guilang_specification *spec = guilang_initspecification("guilang spec.txt");
	char *guidata;
	int len;
	load("testgui.txt", &guidata, &len);
	_guilang_tokenizer_prntphr(guilang_tokenize(guidata, len));
}