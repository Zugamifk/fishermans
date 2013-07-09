typedef struct
_S_guilang_token
{
	_guilang_tokentype type;
	char*	value;
} _guilang_token;

_guilang_token*
_guilang_inittoken
(
	_guilang_tokentype type,
	char* value
)
{
	_guilang_token* token = malloc(sizeof(_guilang_token));
	token->type = type;
	token->value = malloc(GUILANG_WORDLEN);
	strcpy(token->value, value);
	
	return token;
}	

void
_guilang_deletetoken
(
	_guilang_token* token
)
{
	free(token->value);
	free(token);
}

void
_guilang_printtoken
(
	_guilang_token* token
)
{
	const char* type;
	const char* value;
	if (token == NULL) {
		type = "NULL";
		value = "NULL";
	} else {
		type = _guilang_tokentypestrings[token->type];
		value = token->value;
	}
	printf("[ %s, ", type);
	printf("%s ] ", value);
}
