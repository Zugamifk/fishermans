//=============================================================================
//=============================================================================

typedef struct
_S_guilang_grammar
{
	hashtable* rules;
	const char*	startkey;
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
	grammar->rules = hashtable_init(0);
	grammar->startkey = "GUI";
	grammar->version = 2;
	
	#ifdef GUILANG_DEFAULTSPEC
		FILE* langfile = fopen(GUILANG_DEFAULTSPEC, "r");
	#else
		FILE* langfile = fopen(langspec, "r");
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
		
		// parse the tokens
		int success = _guilangspec_parse(tokens, log);
		
		//check semantics
		success &= _guilangspec_analyse(tokens, log);

		// generate rules from the string
		_guilangspec_generaterules(grammar->rules, tokens, log);
		
		_guilangspec_deletetoken(tokens[0]);
		_guilangspec_freelexemes(lexemes);
	}
	
	#if GUILANG_SPEC_PRINTRESULT
		hashtable_print(grammar->rules, _guilang_rule_print);
	#endif
	
	return grammar;
}