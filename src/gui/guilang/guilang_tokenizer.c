bool
_guilang_tokenizer_isnum
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

_guilang_token**
guilang_tokenize
(
	char** lexemes
)
{
	char** word = lexemes;
	
	_guilang_token** tokens = malloc(GUILANG_BIGCAP);
	_guilang_token** curr = tokens;
	
	int endtoken = 0;
	while (!endtoken) {

		if (guilang_iskeyword(*word)) {
			*curr = _guilang_inittoken(GUILANG_KEYWORD, *word);
		} else
		if (_guilang_tokenizer_isnum(*word)) {
			*curr = _guilang_inittoken(GUILANG_NUMBER, *word);
		} else 
		if (strcmp(*word, "$$") == 0) {
			*curr = _guilang_inittoken(GUILANG_ENDOFINPUT, "EOI");
			endtoken = true;
		} else {
			*curr = _guilang_inittoken(GUILANG_STRING, *word);
		}
		
 		curr++;
		word++;
		
	}
	
	return tokens;
}