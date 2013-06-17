// TOKEN TYPES
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

// A TOKEN/STRING PAIR
typedef struct
_S_guilang_tokenpair
{
	_guilang_token token;
	char* string;
} _guilang_tokenpair;
	
_guilang_tokenpair*
guilang_makepair
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
guilang_deletepair
(
	_guilang_tokenpair* pair
)
{
	free(pair->string);
	free(pair);
}


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
guilang_parseword
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
	return guilang_makepair(token, word, holder);
}

void
guilang_init
(
)
{
}