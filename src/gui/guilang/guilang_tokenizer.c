const char reservedchars[] = { '(', ')', '{', '}', ':', ','};
const int reservedcharsnum = 6;

int
_guilang_tokenizer_isnum
(
	char* string
)
{
	int digit = *string - '0';
	return !(digit > 9 || digit < 0);
}

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

int
_guilang_tokenizer_isword
(
	char*	word
)
{
	for (int i = 0; i < _guilang_tokencount; i++) {
		if (strcmp(word, _guilang_tokenwords[i]) == 0) {
			return i;
		}
	}
	return -1;
}

bool
_guilang_tokenizer_iskeyword
(
	char*	word
)
{
	for (int i = 0; i < _guilang_keywordcount; i++) {
		if (strcmp(word, _guilang_keywords[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool
_guilang_tokenizer_isspecialchar
(
	char c
)
{
	for (int i = 0; i < _guilang_specialcharcount; i++) {
		if (c == _guilang_specialchars[i]) return true;
	}
	return false;
}

_guilang_tokenpair*
_guilang_pairid
(
	char*	word
)
{
	int token = _guilang_tokenizer_isword(word);
	if (token >= 0) {
	} else 
	if (_guilang_tokenizer_iskeyword(word)) {
		token = GUILANG_KEYWORD;
	} else
	if (_guilang_tokenizer_isnum(word)) {
		token = GUILANG_NUMBER;
	} else
	if (_guilang_tokenizer_isspecialchar(word[0])) {
		token = GUILANG_SPECIALCHAR;
	}
	return guilang_makepair(token, word, NULL);
	
}

void _guilang_tokenizer_prntphr(void* d) 
{
	_guilang_tokenpair** phrase = (_guilang_tokenpair **) d;
	if (GUILANG_PRINTTOKENSTRINGS) {
		_guilang_tokenpair* t = *phrase;
		while (t->token != GUILANG_ENDOFSTRING) {
			printf("\"%s\" ", t->string);
			t = *(++phrase);
		}
	} else {
		_guilang_tokenpair* t =*(_guilang_tokenpair**)d;
		while (t->token != GUILANG_ENDOFSTRING) {
			printf("%s ", _guilang_tokenstrings[t->token]);
			t = *(++phrase);
		}
	}	
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
	
	char word[GUILANG_LINELEN];
	char *wordptr, *seekptr, *fillptr;
	wordptr=seekptr = NULL;
	fillptr = word;
	
	
	for (int i = 0; i < len; i++)
	{		
		seekptr = data+i;
		if (wordptr == NULL) {
			if (isspace(*seekptr)) {
				continue;
			} else
			if (_guilang_tokenizer_isreservedchar(*seekptr)) {
				sscanf(seekptr, "%c", word);
				word[1] = '\0';
				*currenttoken = _guilang_pairid(word);
				currenttoken++;
			} else {
				wordptr = seekptr;
				*(fillptr++) = *seekptr;
			}
		} else		
		if (	!isspace(*seekptr) && 
				!_guilang_tokenizer_isreservedchar(*seekptr) && 
				isalpha(*wordptr) == isalpha(*seekptr)
			) 
		{
			*(fillptr++) = *seekptr;
		} else {
			*fillptr = '\0';
			*currenttoken = _guilang_pairid(word);
			currenttoken++;
			wordptr = NULL;
			fillptr = word;
			i--;
		}
	
	}
	*currenttoken = _guilang_pairid("ENDOFSTRING");
	
	return tokens;
}