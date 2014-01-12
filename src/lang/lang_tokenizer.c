bool
_lang_tokenizer_isnum
(
	char* word
)
{
	bool decimal = false;
	for (int i = 0; i < strlen(word); i++) {
		if (word[i] == '.' && !decimal) {
			decimal = true;
		} else
		if (!isdigit(word[i])) {
			return false;
		}
	}
	return true;
}

_lang_token**
lang_tokenize
(
	char** lexemes,
	set* keywords
)
{
	char** word = lexemes;
	
	_lang_token** tokens = malloc(LANG_BIGCAP);
	_lang_token** curr = tokens;
	
	int endtoken = 0;
	while (!endtoken) {

		if (_lang_tokenizer_isnum(*word)) {
			*curr = _lang_inittoken(LANG_NUMBER, *word);
		} else 
		if((*word)[0] == '\"') {
			sscanf(*word, "\"%[^\"]\"", *word);
			*curr = _lang_inittoken(LANG_STRING, *word);
		} else
		if (strcmp(*word, "$$") == 0) {
			*curr = _lang_inittoken(LANG_ENDOFINPUT, "EOI");
			endtoken = true;
		} else 
		if (set_has(keywords, *word)) {
			*curr = _lang_inittoken(LANG_KEYWORD, *word);
		} else {
			*curr = _lang_inittoken(LANG_IDENTIFIER, *word);
		}
 		curr++;
		word++;
		
	}
	return tokens;
}