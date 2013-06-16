#define GUILANG_LINELEN 512

typedef enum
_E_guilang_token
{
	GUIHEAD,
	WINDOWHEAD,
	CELLHEAD,
	HEADERPARENOPEN,
	HEADERPARENCLOSE,
	BODYPARENOPEN,
	BODYPARENCLOSE,
	KEYWORDPARAM,
	NUMBER,
	STRING,
	OPERATOR,
	OPTPARENOPEN,
	OPTPARENCLOSE,
	SETPARENOPEN,
	SETPARENCLOSE,
	SETSEP,
	KLEENESTAR,
	NONTERMINAL,
	ENDOFSTRING
} _guilang_token;

typedef struct
_S_guilang_tokenpair
{
	_guilang_token token;
	char* string;
} _guilang_tokenpair;
	
_guilang_tokenpair*
_guilang_makepair
(
	_guilang_token token,
	char* string
)
{
	_guilang_tokenpair *tp = malloc(sizeof(_guilang_tokenpair));
	tp->token = token;
	tp->string = malloc(GUILANG_LINELEN);
	strcpy(tp->string, string);
	
	return tp;
}	

void
_guilang_deletepair
(
	_guilang_tokenpair* pair
)
{
	free(pair->string);
	free(pair);
}

typedef struct
_S_guilang_state
{
	hashtable* grammar;
} guilang_state;

int
_guilang_comparewords
(
	char* str,
	char** words,
	int num
)
{
	char noquotes[GUILANG_LINELEN];
	if (sscanf(str, "\'%[^\']\'", noquotes)==0) return 1;
	for (int i = 0; i < num; i++) {
		if (strcmp(noquotes, words[i]) == 0) return 0;
	}
	return 1;
}

_guilang_token*
_guilang_buildphrase
(
	char*	phrase
)
{
	_guilang_tokenpair** transition = malloc(sizeof(_guilang_tokenpair)*GUILANG_LINELEN);
	char* keywords[] = {"x", "y", "w", "h", "active", "inactive", "horizontal", "vertical"};
	char* operators[] = {":"};
	int i = 0;
	while (*phrase != '\0') {
		char word[GUILANG_LINELEN];
		sscanf(phrase, " %s ", word);
		phrase += strlen(word)+1;
		
		_guilang_token token;
		if (strcmp(word, "GUI") == 0) {
			token = GUIHEAD;
		} else
		if (strcmp(word, "WINDOW") == 0) {
			token = WINDOWHEAD;
		} else
		if (strcmp(word, "CELL") == 0) {
			token = CELLHEAD;
		} else 
		if (strcmp(word, "\'(\'") == 0) {
			token = HEADERPARENOPEN;
		} else
		if (strcmp(word, "\')\'") == 0) {
			token = HEADERPARENCLOSE;
		} else
		if (strcmp(word, "\'{\'") == 0) {
			token = BODYPARENOPEN;
		} else
		if (strcmp(word, "\'}\'") == 0) {
			token = BODYPARENCLOSE;
		} else
		if (_guilang_comparewords(word, keywords, 8) == 0) {
			token = KEYWORDPARAM;
		} else
		if (strcmp(word, "NUMBER") == 0) {
			token = NUMBER;
		} else
		if (strcmp(word, "STRING") == 0) {
			token = STRING;
		} else
		if (_guilang_comparewords(word, operators, 1) == 0) {
			token = OPERATOR;
		} else
		if (strcmp(word, "[") == 0) {
			token = OPTPARENOPEN;
		} else 
		if (strcmp(word, "]") == 0) {
			token = OPTPARENCLOSE;
		} else
		if (strcmp(word, "(") == 0) {
			token = SETPARENOPEN;
		} else
		if (strcmp(word, ")") == 0) {
			token = SETPARENCLOSE;
		} else
		if (strcmp(word, "|") == 0) {
			token = SETSEP;
		} else
		if (strcmp(word, "*") == 0) {
			token = KLEENESTAR;
		} else {
			token = NONTERMINAL;
		}
		transition[i] = _guilang_makepair(token, word);
		printf("%s -- %d \n", word, token);
		i++;
	}

	transition[i] = _guilang_makepair(ENDOFSTRING, "EOS");
	return transition;
}

guilang_state*
guilang_init
(
	const char* langspec
)
{
	guilang_state* state = malloc(sizeof(guilang_state));
	
	state->grammar = hashtable_init(0);
	
	FILE* langfile = fopen(langspec, "r");
	
	while(!feof(langfile)) {
		char line[GUILANG_LINELEN];
		fscanf(langfile, "%[^\n]\n", line);
		char key[GUILANG_LINELEN];
		char transitionstring[GUILANG_LINELEN];
		sscanf(line, "%s -> %[^\n]", key, transitionstring);
		hashtable_insert(state->grammar, key, _guilang_buildphrase(transitionstring));
	}
	
	return state;

}