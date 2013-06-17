const char reservedchars[] = { '(', ')', '{', '}', ':' };
const int reservedcharsnum = 5;

bool
_guilang_tokenizer_isreservedchar
(
	char c
)
{
	for (int i = 0; i < reservedcharsnum; i++) {
		if (c == reservedchars[i]) return true;
	}
	return false;
}

_guilang_tokenpair**
guilang_tokenize
(
	char*	data,
	int		len
)
{
	_guilang_tokenpair** tokens = malloc(GUILANG_LINELEN);
	_guilang_tokenpair** currenttoken = tokens;
	
	char block[GUILANG_LINELEN];
	char word[GUILANG_LINELEN];
	char *wordptr, *seekptr;
	wordptr=seekptr = data;
	
	
	for (int i = 0; i < len; i++)
	{		
		char c = data[i];
		if (isspace(c)) {
			continue;
		} else
		if (_guilang_tokenizer_isreservedchar(c)) {
			word[0] = c;
			word[1] = '\0';
			*currenttoken = guilang_parseword(word, NULL);
			currenttoken++;
		}
	
	}
	
	return tokens;
}