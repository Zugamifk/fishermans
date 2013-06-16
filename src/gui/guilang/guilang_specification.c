#define GUILANG_LINELEN 512
#define GUILANG_DEBUG 0

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

const char* _P_guilang_tokenstrings[] = 
{
	"GUIHEAD",
	"WINDOWHEAD",
	"CELLHEAD",
	"HEADERPARENOPEN",
	"HEADERPARENCLOSE",
	"BODYPARENOPEN",
	"BODYPARENCLOSE",
	"KEYWORDPARAM",
	"NUMBER",
	"STRING",
	"OPERATOR",
	"OPTPARENOPEN",
	"OPTPARENCLOSE",
	"SETPARENOPEN",
	"SETPARENCLOSE",
	"SETSEP",
	"KLEENESTAR",
	"NONTERMINAL",
	"ENDOFSTRING"
};

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
	char* string,
	_guilang_tokenpair* holder
)
{
	_guilang_tokenpair* tp;
	if (holder == NULL) {
		tp = malloc(sizeof(_guilang_tokenpair));
	} else tp = holder;
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
_S_guilang_specification
{
	hashtable* grammar;
	int version;
} guilang_specification;

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

_guilang_tokenpair*
_guilang_parseword
(
	char*	word,
	_guilang_tokenpair*	holder
)
{
	char* keywords[] = {"x", "y", "w", "h", "active", "inactive", "horizontal", "vertical"};
	char* operators[] = {":"};
	_guilang_token token;
	if (strcmp(word, "\'GUI\'") == 0) {
		token = GUIHEAD;
	} else
	if (strcmp(word, "\'WINDOW\'") == 0) {
		token = WINDOWHEAD;
	} else
	if (strcmp(word, "\'CELL\'") == 0) {
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
	if (strcmp(word, "NUMBER") == 0 || atoi(word)) {
		token = NUMBER;
	} else
	if (strcmp(word, "STRING") == 0 || word[0] == '\'') {
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
	return _guilang_makepair(token, word, holder);
}

_guilang_tokenpair**
_guilang_buildphrase
(
	char*	phrase
)
{
	_guilang_tokenpair** transition = malloc(sizeof(_guilang_tokenpair)*GUILANG_LINELEN);
	
	int i = 0;
	while (*phrase != '\0') {
		char word[GUILANG_LINELEN];
		sscanf(phrase, " %s ", word);
		int wordlen = strlen(word);
		phrase += wordlen==strlen(phrase)?wordlen:wordlen+1;
		
		transition[i] = _guilang_parseword(word, NULL);
		i++;
	}

	transition[i] = _guilang_makepair(ENDOFSTRING, "EOS", NULL);
	return transition;
}

#define GUILANG_PRINTTOKENSTRINGS 0
void prntphr(void* d) 
{
	_guilang_tokenpair** phrase = (_guilang_tokenpair **) d;
	if (GUILANG_PRINTTOKENSTRINGS) {
		_guilang_tokenpair* t = *phrase;
		while (t->token != ENDOFSTRING) {
			printf("%s ", t->string);
			t = *(++phrase);
		}
	} else {
		_guilang_tokenpair* t =*(_guilang_tokenpair**)d;
		while (t->token != ENDOFSTRING) {
			printf("%s ", _P_guilang_tokenstrings[t->token]);
			t = *(++phrase);
		}
	}	
}

guilang_specification*
guilang_init
(
	const char* langspec
)
{
	guilang_specification* state = malloc(sizeof(guilang_specification));
	
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
	
	if (GUILANG_DEBUG) hashtable_print(state->grammar, prntphr);

	return state;

}