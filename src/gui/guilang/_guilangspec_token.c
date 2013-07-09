//=============================================================================
typedef struct
_S_guilangspec_token
{
	_guilangspec_tokentype type;
	char* value;
} _guilangspec_token;

_guilangspec_token*
_guilangspec_inittoken
(
	_guilangspec_tokentype type,
	char* value
)
{
	_guilangspec_token* token = malloc(sizeof(_guilangspec_token));
	token->type = type;
	token->value = malloc(GUILANGSPEC_WORDLENMAX);
	strcpy(token->value, value);
	
	return token;
}	

void
_guilangspec_deletetoken
(
	_guilangspec_token* token
)
{
	free(token->value);
	free(token);
}

void
_guilangspec_printtoken
(
	_guilangspec_token* token
)
{
	printf("[ %s, \"%s\" ]\n", _guilangspec_tokentypestrings[token->type], token->value);
}
