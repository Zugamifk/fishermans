// ==================================================
// LANG RULE : 
// Contains a lefthand side and a list of productions
// ==================================================

typedef struct
_S_lang_rule
{
	char*	nonterminal;
	list*	transitions;
	unsigned int transitionct;
} _lang_rule;

typedef struct
_S_lang_rule_production
{
	_lang_token** production;
	unsigned int len;
} _lang_rule_production;

// ==================================================
// TOSTRINGS
// ==================================================

void
_lang_rule_printproduction
(
	_lang_rule_production* production
)
{
	for (int i = 0; i < production->len; i++) {
		_lang_printtoken(production->production[i]);
	}
}

void 
_lang_rule_print
(
	_lang_rule* rule
)
{
	printf("RULE \"%s\":\n", rule->nonterminal);
	for (int i = 0; i < rule->transitionct; i++) {
		printf("-->\t");
		_lang_rule_printproduction(list_get(rule->transitions, i)->data);
		printf("\n");
	}
}

// ==================================================
// CONSTRUCTORS/DESTRUCTORS
// ==================================================
_lang_rule*
_lang_rule_init
(
	const char*	nt 
)
{
	_lang_rule* rule = malloc(sizeof(_lang_rule));
	
	rule->nonterminal = malloc(LANG_WORDLEN);
	strcpy(rule->nonterminal, nt);
	
	rule->transitions = list_new();
	rule->transitionct = 0;
	
	return rule;
}

void
_lang_rule_deleteproduction
(
	_lang_rule_production* production
)
{
	free(production);
}

void
_lang_rule_delete
(
	_lang_rule* rule
)
{
	free(rule->nonterminal);
	list_deepdelete(rule->transitions, (list_deletedatacb)_lang_rule_deleteproduction);
	free(rule);
}

void
_lang_rule_addproduction
(
	_lang_rule* rule,
	_lang_token** string,
	unsigned int len
)
{
	_lang_rule_production* production = malloc(sizeof(_lang_rule_production));
	production->production = malloc(LANG_WORDLEN);
	for (int i = 0; i < len; i++) {
		production->production[i]  = string[i];
	}
	production->len = len;
	list_add(rule->transitions, production);
	rule->transitionct = rule->transitionct +1;
}
