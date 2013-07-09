// =========================================
// SCANNER
// =========================================
char**
_guilangspec_scan
(
	char* in
)
{
	char** lexemes = malloc(sizeof(char*)*GUILANGSPEC_PHRASELENMAX);
	int i = 0;
	while (*in != '\0') {
		lexemes[i] = malloc(GUILANGSPEC_WORDLENMAX);
		sscanf(in, " %s ", lexemes[i]); //bein lazy here
		int wordlen = strlen(lexemes[i]);
		in += wordlen==strlen(in)?wordlen:wordlen+1;
		i++;
	}
	lexemes[i] = "$$";
	return lexemes;
}

void
_guilangspec_freelexemes
(
	char** lexemes
)
{
	for (int i =0; i < GUILANGSPEC_PHRASELENMAX; i++) {
		free(lexemes[i]);
	}
	free(lexemes);
}

// =========================================
// TOKENIZER
// =========================================
_guilangspec_token**
_guilangspec_tokenize
(
	char** lexemes
)
{
	_guilangspec_token** tokens = malloc(sizeof(_guilangspec_token*)*GUILANGSPEC_PHRASELENMAX);
	int ti = 0;
	int li = 0;
	char* word = lexemes[li];
	do {
		// if it's a reserved word, identify it
		for (int i = 0; i < GUILANGSPEC_NUMTOKENTYPES; i++) {
			for (int j = 0; strcmp(_guilangspec_tokensymbols[i][j], "$$") != 0; j++) {
				if (strcmp(_guilangspec_tokensymbols[i][j], word)==0) {
					tokens[ti] = _guilangspec_inittoken(i, word);
					goto next;
				} 
			}
		}
		
		// Try to determine token type by the first character
		char fc = word[0];
		// maybe it's a string
		if ( fc == '\'') {
			tokens[ti] = _guilangspec_inittoken(GUILANGSPEC_TERMINAL, word);
		} else 
		// it's a nonterminal
		{
			tokens[ti] = _guilangspec_inittoken(GUILANGSPEC_NONTERMINAL, word);
		}
		
		// if it's a set seperator and the next element is nothing, insert epsilon
		if ( ((tokens[ti]->type == GUILANGSPEC_ENDOFSTRING) || (tokens[ti]->type == GUILANGSPEC_OPTPARENCLOSE) || (tokens[ti]->type == GUILANGSPEC_SETPARENCLOSE)) && ti > 0 && tokens[ti-1]->type == GUILANGSPEC_SETSEP) {
			tokens[ti+1]  = tokens[ti] ;
			tokens[ti]  = _guilangspec_inittoken(GUILANGSPEC_EPSILON, "");
			ti++;
		} 
		
		next:
		ti++;
		li++;
		word = lexemes[li];
	} while (strcmp(word, "$$") != 0);
	
	tokens[ti] = _guilangspec_inittoken(GUILANGSPEC_ENDOFSTRING, "$$");
	return tokens;
}

// =========================================
// PARSER
// =========================================

void
_guilangspec_parser_error
(
	errorlog* log,
	_guilangspec_token* token,
	_guilangspec_tokentype type
)
{
	char errorstring[1024];
	sprintf(errorstring, "Syntax Error: %s expected, got [ %s ] of type %s", _guilangspec_tokentypestrings[type], token->value, _guilangspec_tokentypestrings[token->type]);
	errorlog_logdef(log, "GUILANG SPECIFICATION PARSER", errorstring);
}

bool
_guilangspec_parse
(
	_guilangspec_token** tokens
,	errorlog* log
)
{
	int i = 0;
	bool success = true;
	while((*tokens)->type != GUILANGSPEC_ENDOFSTRING) {
		switch (i) {
			case 0: {
				if ((*tokens)->type != GUILANGSPEC_NONTERMINAL) {
					_guilangspec_parser_error(log, *tokens, GUILANGSPEC_NONTERMINAL);
					success = false;
				}
				tokens++;
				i++;
			} break;
			case 1: {
				if ((*tokens)->type != GUILANGSPEC_TRANSITIONARROW) {
					_guilangspec_parser_error(log, *tokens, GUILANGSPEC_TRANSITIONARROW);
					success = false;
				}
				tokens++;
				i++;
			} break;
			default: {
				if ((*tokens)->type == GUILANGSPEC_TRANSITIONARROW) {
					errorlog_logdef(log, "GUILANG SPECIFICATION PARSER", "Syntax Error: Transition arrow not allowed in rule body");
					success = false;
				}
				tokens++;
				i++;
			}
		}
	}
	return success;
}

// =========================================
// SEMANTIC ANALYSER
// =========================================

void
_guilangspec_analyser_error
(
	errorlog* log,
	char* errorstring
)
{
	errorlog_logdef(log, "GUILANG SPECIFICATION SEMANTIC ANALYZER", errorstring);
}

bool
_guilangspec_analyse
(
	_guilangspec_token** tokens
,	errorlog* log
)
{
	bool success = false;
	int openparens = 0;
	while((*tokens)->type != GUILANGSPEC_ENDOFSTRING) {
		if (((*tokens)->type == GUILANGSPEC_OPTPARENOPEN) || ((*tokens)->type == GUILANGSPEC_SETPARENOPEN)) {
			openparens++;
		} else 
		if ( ((*tokens)->type == GUILANGSPEC_OPTPARENCLOSE) || ((*tokens)->type == GUILANGSPEC_SETPARENCLOSE) ) {
			openparens--;
		}
		
		tokens++;
	}
	
	// make sure open parens - close parens = 0, or error
	if (openparens != 0) {
		_guilangspec_analyser_error(log, "Unmatched parentheses");
	} else {
		success = true;
	}
	return success;
}

// =========================================
// GRAMMAR GENERATOR
// =========================================
void
_guilangspec_generator_error
(
	errorlog* log,
	const char* message,
	const char* param
)
{
	char errorstring[GUILANG_LINELEN]; 
	sprintf(errorstring, message, param);
	errorlog_logdef(log, "GUILANG GRAMMAR GENERATOR", errorstring);
}

_guilang_token*
_guilangspec_translatetoken
(
	_guilangspec_token* token,
	errorlog* log
)
{
	switch (token->type) {
		case GUILANGSPEC_TERMINAL: {
			char* str = token->value;
			if (strcmp(str, "@") == 0) {
				return _guilang_inittoken(GUILANG_KEYWORD, str);
			} else
			if (strcmp(str, "#") == 0) {
				return _guilang_inittoken(GUILANG_NUMBER, str);
			} else
			{
				return _guilang_inittoken(GUILANG_STRING, str);
			}
		} break;
		case GUILANGSPEC_NONTERMINAL: {
			return _guilang_inittoken(GUILANG_NONTERMINAL, token->value);
		}	break;
		default:
			_guilangspec_generator_error(log, "Expected TERMINAL or NONTERMINAL, got %s", _guilangspec_tokentypestrings[token->type]);
			return NULL;
	}
}

void
_guilangspec_addepsilontransition
(
	_guilang_rule* rule
)
{
	_guilang_token* prod[2] = {
		_guilang_inittoken(GUILANG_EPSILON, "EPSILON"),
		_guilang_inittoken(GUILANG_ENDOFSTRING, "$$")
	};
	_guilang_rule_addproduction(rule, prod, 2);
}

_guilang_token*
_guilangspec_readline
(
	hashtable* grammar
,	char* name
,	_guilangspec_token** tokens
,	errorlog* log
)
{
	_guilang_rule* rule = _guilang_rule_init(name);
	_guilang_token* prod[GUILANG_LINELEN];
	int prodpos = 0;
	
	int inparens = 0;
	int i = 1;
	_guilangspec_token* curr = tokens[i];
	
	int childct = 0;
	
	if (tokens[0]->type == GUILANGSPEC_OPTPARENOPEN) {
		_guilangspec_addepsilontransition(rule);
	}
	
	do {
		if (inparens > 0) {
			if (	curr->type == GUILANGSPEC_OPTPARENOPEN ||
					curr->type == GUILANGSPEC_SETPARENOPEN
				)
			{
				inparens++;
			} else 
			if (	curr->type == GUILANGSPEC_OPTPARENCLOSE ||
					curr->type == GUILANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			}
		} else {
			if (	curr->type == GUILANGSPEC_TERMINAL ||
					curr->type == GUILANGSPEC_NONTERMINAL
				) 
			{
				prod[prodpos] = _guilangspec_translatetoken(curr, log);
				prodpos++;
			} else
			if (	curr->type == GUILANGSPEC_OPTPARENOPEN ||
					curr->type == GUILANGSPEC_SETPARENOPEN
				) 
			{
				char childname[GUILANG_WORDLEN];
				sprintf(childname, "%s-%d", rule->nonterminal, childct);
				childct++;
				prod[prodpos] = _guilangspec_readline(grammar, childname, tokens+i, log);
				prodpos++;
				inparens++;
			} else
			if (	curr->type == GUILANGSPEC_OPTPARENCLOSE ||
					curr->type == GUILANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			} else
			if (curr->type == GUILANGSPEC_SETSEP)
			{
				prod[prodpos] = _guilang_inittoken(GUILANG_ENDOFSTRING, "$$");
				int prodlen = prodpos + 1;
				_guilang_rule_addproduction(rule, prod, prodlen);
				prodpos = 0;
			}
		}
		i++;
		curr = tokens[i];
	} while(inparens >= 0);
	
	prod[prodpos] = _guilang_inittoken(GUILANG_ENDOFSTRING, "$$");
	int prodlen = prodpos + 1;
	_guilang_rule_addproduction(rule, prod, prodlen);
		
	hashtable_insert(grammar, rule->nonterminal, rule);
	
	return _guilang_inittoken(GUILANG_NONTERMINAL, name);
}

void
_guilangspec_generaterules
(
	hashtable*	grammar
,	_guilangspec_token** tokens
,	errorlog* log
)
{	
	_guilang_rule* firstrule = _guilang_rule_init(tokens[0]->value);
	_guilang_token* firstprod[GUILANG_LINELEN];
	int prodpos = 0;
	
	int inparens = 0;
	int i = 2;
	_guilangspec_token* curr = tokens[i];
	
	int childct = 0;
	while(curr->type != GUILANGSPEC_ENDOFSTRING) {
		if (inparens > 0) {
			if (	curr->type == GUILANGSPEC_OPTPARENOPEN ||
					curr->type == GUILANGSPEC_SETPARENOPEN
				)
			{
				inparens++;
			} else 
			if (	curr->type == GUILANGSPEC_OPTPARENCLOSE ||
					curr->type == GUILANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			}
		} else {
			if (	curr->type == GUILANGSPEC_TERMINAL ||
					curr->type == GUILANGSPEC_NONTERMINAL
				) 
			{
				firstprod[prodpos] = _guilangspec_translatetoken(curr, log);
				prodpos++;
			} else
			if (	curr->type == GUILANGSPEC_OPTPARENOPEN ||
					curr->type == GUILANGSPEC_SETPARENOPEN
				) 
			{
				char childname[GUILANG_WORDLEN];
				sprintf(childname, "%s-%d", firstrule->nonterminal, childct);
				childct++;
				firstprod[prodpos] = _guilangspec_readline(grammar, childname, tokens+i, log);
				prodpos++;
				inparens++;
			}
		}
		i++;
		curr = tokens[i];
	}
	
	firstprod[prodpos] = _guilang_inittoken(GUILANG_ENDOFSTRING, curr->value);
	int prodlen = prodpos + 1;
	_guilang_rule_addproduction(firstrule, firstprod, prodlen);
	
	hashtable_insert(grammar, firstrule->nonterminal, firstrule);

}