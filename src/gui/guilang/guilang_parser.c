typedef enum
_E_guilang_parser_error
{
	GUILANG_PARSER_ERROR_NOERROR
,	GUILANG_PARSER_ERROR_TOKENMISMATCH
,	GUILANG_PARSER_ERROR_MAXNESTINGDEPTHEXCEEDED
,	GUILANG_PARSER_ERROR_NONTERMINALUNDEFINED
} guilang_parser_error;

void
guilang_parser_printerror
(
	guilang_parser_error e
,	void*	data
)
{

}

bool
guilang_parser_matchtoken
(
	_guilang_tokenpair* token
,	_guilang_tokenpair* grammar 
)
{
	_guilang_spec_token gt = grammar->token;
	_guilang_token tt = token->token;
	switch (tt) {
		case GUILANG_GUIHEAD:
			return gt == GUILANGSPEC_GUIHEAD;
		case GUILANG_WINDOWHEAD:
			return gt == GUILANGSPEC_WINDOWHEAD;
		case GUILANG_CELLHEAD:
			return gt == GUILANGSPEC_CELLHEAD;
		case GUILANG_KEYWORD:
			return gt == GUILANGSPEC_KEYWORDPARAM;
		case GUILANG_ARGPARENOPEN:
			return gt == GUILANGSPEC_HEADERPARENOPEN;
		case GUILANG_ARGPARENCLOSE:
			return gt == GUILANGSPEC_HEADERPARENCLOSE;
		case GUILANG_BODYPARENOPEN:
			return gt == GUILANGSPEC_BODYPARENOPEN;
		case GUILANG_BODYPARENCLOSE:
			return gt == GUILANGSPEC_BODYPARENCLOSE;
		case GUILANG_NUMBER:
			return gt == GUILANGSPEC_NUMBER;	
		case GUILANG_STRING:
			return gt == GUILANGSPEC_STRING;
		case GUILANG_SPECIALCHAR:
			return gt == GUILANGSPEC_OPERATOR;
		case GUILANG_ENDOFSTRING:
			return gt == GUILANGSPEC_ENDOFSTRING;
	}
	return false;
}

guilang_parser_error
guilang_parse
(
	_guilang_tokenpair** tokens
,	guilang_specification* spec
)
{

	guilang_parser_error err = GUILANG_PARSER_ERROR_NOERROR;
	_guilang_tokenpair** in_curr = tokens;
	
	_guilang_tokenpair** grammar_curr = hashtable_get(spec->grammar, (char*)spec->startkey);
	
	_guilang_tokenpair** parenstack[GUILANG_PARSER_MAXNESTINGDEPTH];
	_guilang_tokenpair*** parenstackpointer = &parenstack[0];
	_guilang_tokenpair*** lastsafeparen = parenstackpointer;
	
	_guilang_tokenpair** nonterminalstack[GUILANG_PARSER_MAXNESTINGDEPTH];
	_guilang_tokenpair*** nonterminalstackpointer = &nonterminalstack[0];
	_guilang_tokenpair*** lastsafenonterminal = nonterminalstackpointer;
	
	bool setfindstack[GUILANG_PARSER_MAXNESTINGDEPTH];
	bool* setfindstackpointer = &setfindstack[0];
	
	_guilang_tokenpair*** seqfail = NULL;

	bool endofinputreached = false;

	while(!endofinputreached && err == GUILANG_PARSER_ERROR_NOERROR) {
		printf("%s\t%s\t %s\n", (*grammar_curr)->string, _P_guilang_spec_tokenstrings[(*grammar_curr)->token], (*in_curr)->string);
		switch ((*grammar_curr)->token) {
			case GUILANGSPEC_GUIHEAD:
			case GUILANGSPEC_WINDOWHEAD:
			case GUILANGSPEC_CELLHEAD:
			case GUILANGSPEC_HEADERPARENOPEN:
			case GUILANGSPEC_HEADERPARENCLOSE:
			case GUILANGSPEC_BODYPARENOPEN:
			case GUILANGSPEC_BODYPARENCLOSE:
			case GUILANGSPEC_KEYWORDPARAM:
			case GUILANGSPEC_NUMBER:
			case GUILANGSPEC_STRING:
			case GUILANGSPEC_OPERATOR:
				if (guilang_parser_matchtoken(*in_curr, *grammar_curr)) {
					in_curr++;
					grammar_curr++;
				} else
				if (parenstackpointer > parenstack) {
					if (seqfail == NULL && *(setfindstackpointer-1) == false) {
						printf("!...");
						seqfail = parenstackpointer;
						grammar_curr = (*lastsafeparen)+1;
						parenstackpointer = lastsafeparen + 1;
						nonterminalstackpointer = lastsafenonterminal;
					}
					grammar_curr++;
				} else
				{
					err = GUILANG_PARSER_ERROR_TOKENMISMATCH;
				} break;
			case GUILANGSPEC_SETPARENOPEN:
				*setfindstackpointer = false;
				setfindstackpointer++;
			case GUILANGSPEC_OPTPARENOPEN: {
				*parenstackpointer = grammar_curr;
				lastsafeparen = parenstackpointer;
				parenstackpointer++;
				grammar_curr++;
				} break;
			case GUILANGSPEC_SETPARENCLOSE: {	
				parenstackpointer--;
				lastsafeparen = parenstackpointer - 1;
				setfindstackpointer--;
				grammar_curr++;
				if (seqfail != NULL) {
					if (parenstackpointer > parenstack) {
					printf ("FAIL!");
						// seqfail stays true for current sequence
					} else
					{
						err = GUILANG_PARSER_ERROR_TOKENMISMATCH;
					} 
				}
			} break;
			case GUILANGSPEC_OPTPARENCLOSE: {
				parenstackpointer--;
				lastsafeparen = parenstackpointer - 1;
				grammar_curr++;
				if (seqfail != NULL && seqfail > parenstackpointer) {
					seqfail = NULL;
				}
			} break;
			case GUILANGSPEC_SETSEP: {
				if (parenstackpointer > parenstack) {
					if (seqfail != NULL ) {
						printf("!!!!\n");
						seqfail = NULL;
					} else {
						*(setfindstackpointer-1) = true;
					}
					*(parenstackpointer-1) = grammar_curr;
					lastsafeparen = parenstackpointer-1;
					grammar_curr++;
				} else {
					err = GUILANG_PARSER_ERROR_TOKENMISMATCH;
				}
			} break;
			case GUILANGSPEC_NONTERMINAL: {
				char* key = (*grammar_curr)->string;
				_guilang_tokenpair** ruledef = hashtable_get(spec->grammar, key);
				if (seqfail != NULL) {
					grammar_curr++;
				} else
				if (ruledef == NULL) {
					printf("%s\t", key);
					err = GUILANG_PARSER_ERROR_NONTERMINALUNDEFINED;
				} else {
					*nonterminalstackpointer = grammar_curr + 1;
					lastsafenonterminal = nonterminalstackpointer;
					nonterminalstackpointer++;
					grammar_curr = ruledef;
				}
			} break;
			case GUILANGSPEC_ENDOFSTRING: {
				if (nonterminalstackpointer > nonterminalstack) {
					nonterminalstackpointer--;
					grammar_curr = *(nonterminalstackpointer-1);
					printf("?!?!");
				} else
				if (guilang_parser_matchtoken(*in_curr, *grammar_curr)) {
					endofinputreached = true;
					printf("!!!");
				}
			} break;
		}
	}
	printf("YAY!!!!!! %d \n", err);
	return err;
}