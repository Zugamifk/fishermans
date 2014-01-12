void _lang_printstr(char* s) {printf("%s", s);}
//=============================================================================

typedef struct
_S_lang_grammar
{
	set* keywords;
	set* terminals;
	set* nonterminals;
	hashtable* rules;
	_lang_token*	startsymbol;
	int version;
} lang_grammar;

lang_grammar*
lang_initgrammar
(
	const char* filename
,	errorlog* log
)
{
	lang_grammar* grammar = malloc(sizeof(lang_grammar));
	grammar->keywords = set_initcb((set_cmpcb)strcmp);
	grammar->terminals = set_initcb((set_cmpcb)strcmp);
	grammar->nonterminals = set_initcb((set_cmpcb)strcmp);
	grammar->rules = hashtable_init(0);
	grammar->version = 2;
	
	#ifdef LANG_DEFAULTSPEC
		FILE* langfile = fopen(LANG_DEFAULTSPEC, "r");
	#else
		FILE* langfile = fopen(filename, "r");
	#endif

	bool startsymbolset = false;
	
	// parse specification
	while(!feof(langfile)) {
	
		// scan one line at a time
		char line[LANG_LINELEN];
		fscanf(langfile, "%[^\n]\n", line);
		
		// separate line into lexemes (words)
		char** lexemes = _langspec_scan(line);

		// tokenize the lexemes		
		_langspec_token** tokens = _langspec_tokenize(lexemes);
		
		
		//get terminals out of tokens
		for (int i = 0; tokens[i]->type != LANGSPEC_ENDOFSTRING; i++) {
			if (tokens[i]->type == LANGSPEC_TERMINAL) {
				set_add(grammar->terminals, tokens[i]->value);
			}
		}
		
		// parse the tokens
		int success = _langspec_parse(tokens, log);
		
		//check semantics
		success &= _langspec_analyse(tokens, log);
		
		// generate rules from the string
		_langspec_generaterules(grammar->rules, grammar->keywords, tokens, log);
		
		// set the start token
		if (!startsymbolset) {
			grammar->startsymbol = _lang_inittoken(LANG_NONTERMINAL, tokens[0]->value);
			startsymbolset = true;
		}

	//	_langspec_deletetoken(tokens[0]);
	//	_langspec_freelexemes(lexemes);
	}
	
	set_add(grammar->terminals, LANG_ENDOFINPUTSTRING);
	
	HASHTABLEDATA* r;
	char* k;
	for(	hashtable_begin(grammar->rules, &k, &r); 
			hashtable_end(grammar->rules); 
			hashtable_next(grammar->rules, &k, &r))
		{
			set_add(grammar->nonterminals, k);
		}
	
	#ifdef LANG_SPEC_PRINTRESULT
		set_print(grammar->terminals, (set_printcb)_lang_printstr);
		set_print(grammar->nonterminals, (set_printcb)_lang_printstr);
		hashtable_print(grammar->rules, (hashtable_printcb)_lang_rule_print);
	#endif

	return grammar;
}

//=============================================================================

