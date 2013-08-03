//=============================================================================
typedef struct
_S_langspec_token
{
	_langspec_tokentype type;
	char* value;
} _langspec_token;

_langspec_token*
_langspec_inittoken
(
	_langspec_tokentype type,
	char* value
)
{
	_langspec_token* token = malloc(sizeof(_langspec_token));
	token->type = type;
	token->value = malloc(LANGSPEC_WORDLENMAX);
	strcpy(token->value, value);
	
	return token;
}	

void
_langspec_deletetoken
(
	_langspec_token* token
)
{
	free(token->value);
	free(token);
}

void
_langspec_printtoken
(
	_langspec_token* token
)
{
	printf("[ %s, \"%s\" ]\n", _langspec_tokentypestrings[token->type], token->value);
}
