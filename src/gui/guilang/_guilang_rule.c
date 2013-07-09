// ==================================================
// GUILANG RULE : 
// Contains a lefthand side and a list of productions
// ==================================================
typedef struct
_S_guilang_rule
{
	char*	nonterminal;
	list*	transitions;
	unsigned int transitionct;
} _guilang_rule;

typedef struct
_S_guilang_rule_production
{
	_guilang_token** production;
	unsigned int len;
} _guilang_rule_production;

// ==================================================
// CONSTRUCTORS/DESTRUCTORS
// ==================================================
_guilang_rule*
_guilang_rule_init
(
	const char*	nt 
)
{
	_guilang_rule* rule = malloc(sizeof(_guilang_rule));
	
	rule->nonterminal = malloc(GUILANG_WORDLEN);
	strcpy(rule->nonterminal, nt);
	
	rule->transitions = list_new();
	rule->transitionct = 0;
	
	return rule;
}

void
_guilang_rule_deleteproduction
(
	_guilang_rule_production* production
)
{
	free(production);
}

void
_guilang_rule_delete
(
	_guilang_rule* rule
)
{
	free(rule->nonterminal);
	list_deepdelete(rule->transitions, (list_deletedatacb)_guilang_rule_deleteproduction);
	free(rule);
}

void
_guilang_rule_addproduction
(
	_guilang_rule* rule,
	_guilang_token** string,
	unsigned int len
)
{
	_guilang_rule_production* production = malloc(sizeof(_guilang_rule_production));
	production->production = malloc(GUILANG_WORDLEN);
	for (int i = 0; i < len; i++) {
		production->production[i]  = string[i];
	}
	production->len = len;
	list_add(rule->transitions, production);
	rule->transitionct = rule->transitionct +1;

}

// ==================================================
// TOSTRINGS
// ==================================================

void
_guilang_rule_printproduction
(
	_guilang_rule_production* production
)
{
	for (int i = 0; i < production->len; i++) {
		_guilang_printtoken(production->production[i]);
	}
}

void 
_guilang_rule_print
(
	_guilang_rule* rule
)
{
	printf("RULE \"%s\":\n", rule->nonterminal);
	for (int i = 0; i < rule->transitionct; i++) {
		printf("\t");
		_guilang_rule_printproduction(list_get(rule->transitions, i)->data);
		printf("\n");
	}
}