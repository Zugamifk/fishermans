// =========================================
// SCANNER
// =========================================
char**
_langspec_scan
(
	char* in
)
{
	char** lexemes = malloc(sizeof(char*)*LANGSPEC_PHRASELENMAX);
	int i = 0;
	while (*in != '\0') {
		lexemes[i] = malloc(LANGSPEC_WORDLENMAX);
		sscanf(in, " %s ", lexemes[i]); //bein lazy here
		int wordlen = strlen(lexemes[i]);
		in += wordlen==strlen(in)?wordlen:wordlen+1;
		i++;
	}
	lexemes[i] = "$$";
	return lexemes;
}

void
_langspec_freelexemes
(
	char** lexemes
)
{
	for (int i =0; i < LANGSPEC_PHRASELENMAX; i++) {
		free(lexemes[i]);
	}
	free(lexemes);
}

// =========================================
// TOKENIZER
// =========================================
_langspec_token**
_langspec_tokenize
(
	char** lexemes
)
{
	_langspec_token** tokens = malloc(sizeof(_langspec_token*)*LANGSPEC_PHRASELENMAX);
	int ti = 0;
	int li = 0;
	char* word = lexemes[li];
	do {
		// if it's a reserved word, identify it
		for (int i = 0; i < LANGSPEC_NUMTOKENTYPES; i++) {
			for (int j = 0; strcmp(_langspec_tokensymbols[i][j], "$$") != 0; j++) {
				if (strcmp(_langspec_tokensymbols[i][j], word)==0) {
					tokens[ti] = _langspec_inittoken(i, word);
					goto next;
				} 
			}
		}
		
		// Try to determine token type by the first character
		char fc = word[0];
		// maybe it's a string
		if ( fc == '\'') {
			sscanf(word, "\'%[^\']\'", word);
			tokens[ti] = _langspec_inittoken(LANGSPEC_TERMINAL, word);
		} else 
		// it's a nonterminal
		{
			tokens[ti] = _langspec_inittoken(LANGSPEC_NONTERMINAL, word);
		}
		
		// if it's a set seperator and the next element is nothing, insert epsilon
		if ( ((tokens[ti]->type == LANGSPEC_ENDOFSTRING) || (tokens[ti]->type == LANGSPEC_OPTPARENCLOSE) || (tokens[ti]->type == LANGSPEC_SETPARENCLOSE)) && ti > 0 && tokens[ti-1]->type == LANGSPEC_SETSEP) {
			tokens[ti+1]  = tokens[ti] ;
			tokens[ti]  = _langspec_inittoken(LANGSPEC_EPSILON, "");
			ti++;
		} 
		
		next:
		ti++;
		li++;
		word = lexemes[li];
	} while (strcmp(word, "$$") != 0);
	
	tokens[ti] = _langspec_inittoken(LANGSPEC_ENDOFSTRING, "$$");
	return tokens;
}

// =========================================
// PARSER
// =========================================

void
_langspec_parser_error
(
	errorlog* log,
	_langspec_token* token,
	_langspec_tokentype type
)
{
	char errorstring[1024];
	sprintf(errorstring, "Syntax Error: %s expected, got [ %s ] of type %s", _langspec_tokentypestrings[type], token->value, _langspec_tokentypestrings[token->type]);
	errorlog_logdef(log, "LANG SPECIFICATION PARSER", errorstring);
}

bool
_langspec_parse
(
	_langspec_token** tokens
,	errorlog* log
)
{
	int i = 0;
	bool success = true;
	while((*tokens)->type != LANGSPEC_ENDOFSTRING) {
		switch (i) {
			case 0: {
				if ((*tokens)->type != LANGSPEC_NONTERMINAL) {
					_langspec_parser_error(log, *tokens, LANGSPEC_NONTERMINAL);
					success = false;
				}
				tokens++;
				i++;
			} break;
			case 1: {
				if ((*tokens)->type != LANGSPEC_TRANSITIONARROW) {
					_langspec_parser_error(log, *tokens, LANGSPEC_TRANSITIONARROW);
					success = false;
				}
				tokens++;
				i++;
			} break;
			default: {
				if ((*tokens)->type == LANGSPEC_TRANSITIONARROW) {
					errorlog_logdef(log, "LANG SPECIFICATION PARSER", "Syntax Error: Transition arrow not allowed in rule body");
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
_langspec_analyser_error
(
	errorlog* log,
	char* errorstring
)
{
	errorlog_logdef(log, "LANG SPECIFICATION SEMANTIC ANALYZER", errorstring);
}

bool
_langspec_analyse
(
	_langspec_token** tokens
,	errorlog* log
)
{
	bool success = false;
	int openparens = 0;
	while((*tokens)->type != LANGSPEC_ENDOFSTRING) {
		if (((*tokens)->type == LANGSPEC_OPTPARENOPEN) || ((*tokens)->type == LANGSPEC_SETPARENOPEN)) {
			openparens++;
		} else 
		if ( ((*tokens)->type == LANGSPEC_OPTPARENCLOSE) || ((*tokens)->type == LANGSPEC_SETPARENCLOSE) ) {
			openparens--;
		}
		
		tokens++;
	}
	
	// make sure open parens - close parens = 0, or error
	if (openparens != 0) {
		_langspec_analyser_error(log, "Unmatched parentheses");
	} else {
		success = true;
	}
	return success;
}

// =========================================
// GRAMMAR GENERATOR
// =========================================
void
_langspec_generator_error
(
	errorlog* log,
	const char* message,
	const char* param
)
{
	char errorstring[LANG_LINELEN]; 
	sprintf(errorstring, message, param);
	errorlog_logdef(log, "LANG GRAMMAR GENERATOR", errorstring);
}

_lang_token*
_langspec_translatetoken
(
	_langspec_token* token,
	errorlog* log
)
{
	switch (token->type) {
		case LANGSPEC_TERMINAL: {
			char* str = token->value;
			if (strcmp(str, "@") == 0) {
				return _lang_inittoken(LANG_KEYWORD, str);
			} else
			if (strcmp(str, "#") == 0) {
				return _lang_inittoken(LANG_NUMBER, str);
			} else
			{
				//sscanf(str, "\'%[^\']\'", str);
				return _lang_inittoken(LANG_STRING, str);
			}
		} break;
		case LANGSPEC_NONTERMINAL: {
			return _lang_inittoken(LANG_NONTERMINAL, token->value);
		}	break;
		default:
			_langspec_generator_error(log, "Expected TERMINAL or NONTERMINAL, got %s", _langspec_tokentypestrings[token->type]);
			return NULL;
	}
}

void
_langspec_addepsilontransition
(
	_lang_rule* rule
)
{
	_lang_token* prod[2] = {
		_lang_inittoken(LANG_EPSILON, "EPSILON"),
		_lang_inittoken(LANG_ENDOFSTRING, "$$")
	};
	_lang_rule_addproduction(rule, prod, 2);
}

_lang_token*
_langspec_readline
(
	hashtable* grammar
,	char* name
,	_langspec_token** tokens
,	errorlog* log
)
{
	_lang_rule* rule = _lang_rule_init(name);
	_lang_token* prod[LANG_LINELEN];
	int prodpos = 0;
	
	int inparens = 0;
	int i = 1;
	_langspec_token* curr = tokens[i];
	
	int childct = 0;

	bool addedepsilon = false;
	
	do {
		if (inparens > 0) {
			if (	curr->type == LANGSPEC_OPTPARENOPEN ||
					curr->type == LANGSPEC_SETPARENOPEN
				)
			{
				inparens++;
			} else 
			if (	curr->type == LANGSPEC_OPTPARENCLOSE ||
					curr->type == LANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			}
		} else {
			if (	curr->type == LANGSPEC_TERMINAL ||
					curr->type == LANGSPEC_NONTERMINAL ||
					curr->type == LANGSPEC_EPSILON
				) 
			{
				if ( curr->type == LANGSPEC_EPSILON) addedepsilon = true;
				prod[prodpos] = _langspec_translatetoken(curr, log);
				prodpos++;
			} else
			if (	curr->type == LANGSPEC_OPTPARENOPEN ||
					curr->type == LANGSPEC_SETPARENOPEN
				) 
			{
				char childname[LANG_WORDLEN];
				sprintf(childname, "%s-%d", rule->nonterminal, childct);
				childct++;
				prod[prodpos] = _langspec_readline(grammar, childname, tokens+i, log);
				prodpos++;
				inparens++;
			} else
			if (	curr->type == LANGSPEC_OPTPARENCLOSE ||
					curr->type == LANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			} else
			if (curr->type == LANGSPEC_SETSEP)
			{
				prod[prodpos] = _lang_inittoken(LANG_ENDOFSTRING, "$$");
				int prodlen = prodpos + 1;
				_lang_rule_addproduction(rule, prod, prodlen);
				prodpos = 0;
			}
		}
		i++;
		curr = tokens[i];
	} while(inparens >= 0);
	
	if (!addedepsilon && (prodpos == 0 || tokens[0]->type == LANGSPEC_OPTPARENOPEN) ) {
		_langspec_addepsilontransition(rule);
	}
	prod[prodpos] = _lang_inittoken(LANG_ENDOFSTRING, "$$");
	int prodlen = prodpos + 1;
	_lang_rule_addproduction(rule, prod, prodlen);
		
	hashtable_insert(grammar, rule->nonterminal, rule);
	
	return _lang_inittoken(LANG_NONTERMINAL, name);
}

void
_langspec_generaterules
(
	hashtable*	grammar
,	_langspec_token** tokens
,	errorlog* log
)
{	
	_lang_rule* firstrule = _lang_rule_init(tokens[0]->value);
	_lang_token* firstprod[LANG_LINELEN];
	int prodpos = 0;
	
	int inparens = 0;
	int i = 2;
	_langspec_token* curr = tokens[i];
	
	int childct = 0;
	while(curr->type != LANGSPEC_ENDOFSTRING) {
		if (inparens > 0) {
			if (	curr->type == LANGSPEC_OPTPARENOPEN ||
					curr->type == LANGSPEC_SETPARENOPEN
				)
			{
				inparens++;
			} else 
			if (	curr->type == LANGSPEC_OPTPARENCLOSE ||
					curr->type == LANGSPEC_SETPARENCLOSE
				)
			{
				inparens--;
			}
		} else {
			if (	curr->type == LANGSPEC_TERMINAL ||
					curr->type == LANGSPEC_NONTERMINAL
				) 
			{
				firstprod[prodpos] = _langspec_translatetoken(curr, log);
				prodpos++;
			} else
			if (	curr->type == LANGSPEC_OPTPARENOPEN ||
					curr->type == LANGSPEC_SETPARENOPEN
				) 
			{
				char childname[LANG_WORDLEN];
				sprintf(childname, "%s-%d", firstrule->nonterminal, childct);
				childct++;
				firstprod[prodpos] = _langspec_readline(grammar, childname, tokens+i, log);
				prodpos++;
				inparens++;
			}
		}
		i++;
		curr = tokens[i];
	}
	
	//firstprod[prodpos] = _lang_inittoken(LANG_ENDOFINPUT, curr->value);
	firstprod[prodpos] = _lang_inittoken(LANG_ENDOFSTRING, curr->value);
	int prodlen = prodpos + 1;
	_lang_rule_addproduction(firstrule, firstprod, prodlen);
	
	hashtable_insert(grammar, firstrule->nonterminal, firstrule);

}