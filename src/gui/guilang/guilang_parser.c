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
	
	stack* parenstack = stack_init(0);
	stack* nonterminalstack = stack_init(0);
	stack* setfindstack = stack_init(0);
	
	_guilang_tokenpair** seqfail = NULL;

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
			case GUILANGSPEC_OPERATOR: {
				if (seqfail == NULL) {
					if (guilang_parser_matchtoken(*in_curr, *grammar_curr)) {
						in_curr++;
						grammar_curr++;
					} else
					if (stack_isempty(parenstack)) {
						err = GUILANG_PARSER_ERROR_TOKENMISMATCH;
					} else 
					if (stack_peek(setfindstack)) {
						grammar_curr++;
					} else{
						seqfail = stack_peek(parenstack);
						grammar_curr++;
					}
				} else {
					grammar_curr++;
				}
			} break;
			case GUILANGSPEC_SETPARENOPEN:
				stack_push(setfindstack, false);
			case GUILANGSPEC_OPTPARENOPEN: {
				grammar_curr++;
				stack_push(parenstack, grammar_curr);
			} break;
			case GUILANGSPEC_SETPARENCLOSE: {
				stack_pop(setfindstack);
				stack_pop(parenstack);
				grammar_curr++;
			} break;
			case GUILANGSPEC_OPTPARENCLOSE: {
				_guilang_tokenpair** lastparen = stack_pop(parenstack);
				grammar_curr++;
				if (seqfail >= lastparen) {
					seqfail = NULL;
				}
			} break;
			case GUILANGSPEC_SETSEP: {
				stack_pop(parenstack);
				stack_push(parenstack, grammar_curr);
				grammar_curr++;
				if (seqfail == NULL) {
					stack_pop(setfindstack);
					stack_push(setfindstack, (void *)true);
				}
			} break;
			case GUILANGSPEC_NONTERMINAL: {
				char* key = (*grammar_curr)->string;
				_guilang_tokenpair** ruledef = hashtable_get(spec->grammar, key);
				if (stack_peek(setfindstack)) {
					grammar_curr++;
				} else
				if (ruledef == NULL) {
					err = GUILANG_PARSER_ERROR_NONTERMINALUNDEFINED;
				} else
				if (seqfail == NULL) {
					grammar_curr++;
					stack_push(nonterminalstack, grammar_curr);
					grammar_curr = ruledef;
				} else {
					grammar_curr++;
				}
			} break;
			case GUILANGSPEC_ENDOFSTRING: {
				_guilang_tokenpair** last = stack_pop(nonterminalstack);
				if (last == NULL) {
					endofinputreached = true;
				} else {
					grammar_curr = last;
				}
			} break;
		}
	}
	printf("YAY!!!!!! %d \n", err);
	return err;
}