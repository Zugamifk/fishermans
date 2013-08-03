typedef struct
_S_lang_token
{
	_lang_tokentype type;
	char*	value;
} _lang_token;

_lang_token*
_lang_inittoken
(
	_lang_tokentype type,
	char* value
)
{
	_lang_token* token = malloc(sizeof(_lang_token));
	token->type = type;
	token->value = malloc(LANG_WORDLEN);
	strcpy(token->value, value);
	
	return token;
}	

void
_lang_deletetoken
(
	_lang_token* token
)
{
	free(token->value);
	free(token);
}

void
_lang_printtoken
(
	_lang_token* token
)
{
	const char* type;
	const char* value;
	if (token == NULL) {
		type = "NULL";
		value = "NULL";
	} else {
		type = _lang_tokentypestrings[token->type];
		value = token->value;
	}
	printf("[ %s, ", type);
	printf("%s ] ", value);
}
