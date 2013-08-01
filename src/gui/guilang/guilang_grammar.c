//=============================================================================
bool
guilang_iskeyword
(
	const char* word
)
{
	for (int i = 0; i < _guilang_keywordcount; i++) {
		if (strcmp(_guilang_keywords[i], word) == 0) {
			return true;
		}
	}
	return false;
}

void _guilang_printstr(char* s) {printf("%s", s);}
//=============================================================================

typedef struct
_S_guilang_grammar
{
	set* terminals;
	set* nonterminals;
	hashtable* rules;
	_guilang_token*	startsymbol;
	int version;
} guilang_grammar;

guilang_grammar*
guilang_initgrammar
(
	const char* filename
,	errorlog* log
)
{
	guilang_grammar* grammar = malloc(sizeof(guilang_grammar));
	grammar->terminals = set_initcb((set_cmpcb)strcmp);
	grammar->nonterminals = set_initcb((set_cmpcb)strcmp);
	grammar->rules = hashtable_init(0);
	grammar->startsymbol = _guilang_inittoken(GUILANG_NONTERMINAL, GUILANG_STARTSYMBOL);
	grammar->version = 2;
	
	#ifdef GUILANG_DEFAULTSPEC
		FILE* langfile = fopen(GUILANG_DEFAULTSPEC, "r");
	#else
		FILE* langfile = fopen(filename, "r");
	#endif

	// parse specification
	while(!feof(langfile)) {
	
		// scan one line at a time
		char line[GUILANG_LINELEN];
		fscanf(langfile, "%[^\n]\n", line);
		
		// separate line into lexemes (words)
		char** lexemes = _guilangspec_scan(line);

		// tokenize the lexemes		
		_guilangspec_token** tokens = _guilangspec_tokenize(lexemes);
		
		//get terminals out of tokens
		for (int i = 0; tokens[i]->type != GUILANGSPEC_ENDOFSTRING; i++) {
			if (tokens[i]->type == GUILANGSPEC_TERMINAL) {
				set_add(grammar->terminals, tokens[i]->value);
				set_print(grammar->terminals, (set_printcb)_guilang_printstr);
			}
		}
		
		// parse the tokens
		int success = _guilangspec_parse(tokens, log);
						set_print(grammar->terminals, (set_printcb)_guilang_printstr);

		//check semantics
		success &= _guilangspec_analyse(tokens, log);
				set_print(grammar->terminals, (set_printcb)_guilang_printstr);

		// generate rules from the string
		_guilangspec_generaterules(grammar->rules, tokens, log);
						set_print(grammar->terminals, (set_printcb)_guilang_printstr);

	//	_guilangspec_deletetoken(tokens[0]);
	//	_guilangspec_freelexemes(lexemes);
						set_print(grammar->terminals, (set_printcb)_guilang_printstr);

	}
	
	HASHTABLEDATA* r;
	char* k;
	for(	hashtable_begin(grammar->rules, &k, &r); 
			hashtable_end(grammar->rules); 
			hashtable_next(grammar->rules, &k, &r))
		{
			set_add(grammar->nonterminals, k);
		}
	
	#ifdef GUILANG_SPEC_PRINTRESULT
		set_print(grammar->terminals, (set_printcb)_guilang_printstr);
		set_print(grammar->nonterminals, (set_printcb)_guilang_printstr);
		hashtable_print(grammar->rules, (hashtable_printcb)_guilang_rule_print);
	#endif
					set_print(grammar->terminals, (set_printcb)_guilang_printstr);

	return grammar;
}

//=============================================================================

