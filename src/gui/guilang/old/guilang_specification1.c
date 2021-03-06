typedef struct
_S_guilang_specification
{
	hashtable* grammar;
	const char*	startkey;
	int version;
} guilang_specification;

_guilang_tokenpair*
guilang_parseword
(
	char*	word,
	_guilang_tokenpair*	holder
)
{
	char* keywords[] = {"x", "y", "w", "h", "active", "inactive", "horizontal", "vertical"};
	char* operators[] = {":", ","};
	_guilang_spec_token token;
	if (strcmp(word, "\'GUI\'") == 0) {
		token = GUILANGSPEC_GUIHEAD;
	} else
	if (strcmp(word, "\'WINDOW\'") == 0) {
		token = GUILANGSPEC_WINDOWHEAD;
	} else
	if (strcmp(word, "\'CELL\'") == 0) {
		token = GUILANGSPEC_CELLHEAD;
	} else 
	if (strcmp(word, "\'(\'") == 0) {
		token = GUILANGSPEC_HEADERPARENOPEN;
	} else
	if (strcmp(word, "\')\'") == 0) {
		token = GUILANGSPEC_HEADERPARENCLOSE;
	} else
	if (strcmp(word, "\'{\'") == 0) {
		token = GUILANGSPEC_BODYPARENOPEN;
	} else
	if (strcmp(word, "\'}\'") == 0) {
		token = GUILANGSPEC_BODYPARENCLOSE;
	} else
	if (_guilang_comparewords(word, keywords, 8) == 0) {
		token = GUILANGSPEC_KEYWORDPARAM;
	} else
	if (strcmp(word, "NUMBER") == 0 || atoi(word) || atof(word)) {
		token = GUILANGSPEC_NUMBER;
	} else
	if (_guilang_comparewords(word, operators, 2) == 0) {
		token = GUILANGSPEC_OPERATOR;
	} else
	if (strcmp(word, "STRING") == 0 || word[0] == '\'') {
		token = GUILANGSPEC_STRING;
	} else
	if (strcmp(word, "[") == 0) {
		token = GUILANGSPEC_OPTPARENOPEN;
	} else 
	if (strcmp(word, "]") == 0) {
		token = GUILANGSPEC_OPTPARENCLOSE;
	} else
	if (strcmp(word, "(") == 0) {
		token = GUILANGSPEC_SETPARENOPEN;
	} else
	if (strcmp(word, ")") == 0) {
		token = GUILANGSPEC_SETPARENCLOSE;
	} else
	if (strcmp(word, "|") == 0) {
		token = GUILANGSPEC_SETSEP;
	} else
	if (strcmp(word, "ENDOFSTRING") == 0) {
		token = GUILANGSPEC_ENDOFSTRING;
	} else {
		token = GUILANGSPEC_NONTERMINAL;
	}
	return guilang_makepair(token, word, holder);
}

void
_guilang_buildrules
(
	_guilang_tokenpair** rule
,	hashtable* grammar
)
{
	_guilang_tokenpair** newrule = malloc(sizeof(_guilang_tokenpair)*GUILANG_LINELEN);
	_guilang_tokenpair* curr = rule[0];
	int i = 0;
	while(curr->token != GUILANGSPEC_ENDOFSTRING)
	{
		if (curr->token
	}
}
	
void
_guilang_parserule
(
	char*	phrase
,	hashtable*	grammar
)
{
	_guilang_tokenpair *rule[GUILANG_LINELEN];
	int i = 0;
	
	while (*phrase != '\0') {
		char word[GUILANG_LINELEN];
		sscanf(phrase, " %s ", word);
		int wordlen = strlen(word);
		phrase += wordlen==strlen(phrase)?wordlen:wordlen+1;
		
		rule[i] = guilang_parseword(word, NULL);
		i++;
	}

	rule[i] = guilang_parseword("ENDOFSTRING", NULL);
	_guilang_buildrules(rule, grammar);
}

void prntphr(void* d) 
{
	_guilang_tokenpair** phrase = (_guilang_tokenpair **) d;
	if (GUILANG_PRINTTOKENSTRINGS) {
		_guilang_tokenpair* t = *phrase;
		while (t->token != GUILANGSPEC_ENDOFSTRING) {
			printf("\"%s\" ", t->string);
			t = *(++phrase);
		}
	} else {
		_guilang_tokenpair* t =*(_guilang_tokenpair**)d;
		while (t->token != GUILANGSPEC_ENDOFSTRING) {
			printf("%s ", _P_guilang_spec_tokenstrings[t->token]);
			t = *(++phrase);
		}
	}	
}

guilang_specification*
guilang_initspecification
(
	const char* langspec
)
{
	guilang_specification* state = malloc(sizeof(guilang_specification));
	
	state->grammar = hashtable_init(0);
	state->startkey = "GUI";
	
	#ifdef GUILANG_DEFAULTSPEC
		FILE* langfile = fopen(GUILANG_DEFAULTSPEC, "r");
	#else
		FILE* langfile = fopen(langspec, "r");
	#endif
	
	while(!feof(langfile)) {
		char line[GUILANG_LINELEN];
		fscanf(langfile, "%[^\n]\n", line);
		char key[GUILANG_LINELEN];
		char transitionstring[GUILANG_LINELEN];
		sscanf(line, "%s -> %[^\n]", key, transitionstring);
	//	hashtable_insert(state->grammar, key, _guilang_buildphrase(transitionstring));
	}
	
	#if GUILANG_SPEC_PRINTRESULT
		hashtable_print(state->grammar, prntphr);
	#endif
	
	return state;

}