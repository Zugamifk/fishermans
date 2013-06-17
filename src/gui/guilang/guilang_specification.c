typedef struct
_S_guilang_specification
{
	hashtable* grammar;
	int version;
} guilang_specification;


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
		
		transition[i] = guilang_parseword(word, NULL);
		i++;
	}

	transition[i] = guilang_makepair(ENDOFSTRING, "EOS", NULL);
	return transition;
}

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
guilang_initspecification
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