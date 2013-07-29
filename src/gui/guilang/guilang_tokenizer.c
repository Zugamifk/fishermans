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
		if (strtol(*word, NULL, 10) || strtof(*word, NULL)) {
			*curr = _guilang_inittoken(GUILANG_NUMBER, *word);
		} else 
		if (strcmp(*word, "$$") == 0) {
			*curr = _guilang_inittoken(GUILANG_ENDOFINPUT, *word);
			endtoken = true;
		} else {
			*curr = _guilang_inittoken(GUILANG_STRING, *word);
		}
		curr++;
		word++;
		
	}
	
	return tokens;
}