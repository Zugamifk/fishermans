// ==================================================
// GUILANG PARSER : 
// Builds a parsing table with an pre-constructed grammar.
// Does checks for ambiguities in the grammar.
// ==================================================

// DEFINITIONS
// ==================================================
typedef struct
_S_guilang_parsingtable
{
	hashtable* nti;
} _guilang_parsingtable;

// PRINT FUNCTIONS
// ==================================================

void
_guilang_parser_shtstr
(
	char* w
)
{
	int l = strlen(w);
	if (l < 4) {
		printf("%s", w);
	} else {
		int m = l/2;
		printf("%c%c%c", w[0], w[m], w[l-1]);
	}
}

void
_guilang_parser_printset
(
	void* d
)
{
	set_print((set*)d, (set_printcb)_guilang_printstr);
}

void
_guilang_parser_printsetlist
(
	void* d
)
{
	list* l = (list*)d;
	printf("\n");
	while(l->data!=NULL) {
	printf("\t\t");
		set_print((set*)l->data, (set_printcb)_guilang_printstr);
		l = l->next;
	}
}

void
_guilang_parser_printparsingtable
(
	_guilang_parsingtable* t
)
{
	HASHTABLEDATA* ti;
	char* nt;
	for(hashtable_begin(t->nti, &nt, &ti);
		hashtable_end(t->nti);
		hashtable_next(t->nti, &nt, &ti))
	{
		_guilang_parser_shtstr(nt);
		HASHTABLEDATA* predict;
		char* tt;
		for(hashtable_begin((hashtable*)ti, &tt, &predict);
			hashtable_end((hashtable*)ti);
			hashtable_next((hashtable*)ti, &tt, &predict))
		{
			_guilang_rule_production* rule = (_guilang_rule_production*)predict;
			printf("\t[");
			if (rule != NULL) {
				char* str = rule->production[0]->value;
				_guilang_parser_shtstr(str);
			}
			printf("]");
		}
		printf("\n");
	}
}

// PARSING TABLE
// ==================================================
_guilang_parsingtable*
_guilang_parser_initparsingtable
(
	guilang_grammar* grammar
)
{
	hashtable* ntfirst = hashtable_init(0);
	
	set* temp = set_init();
	set* eps = set_init();
	set_add(eps, "EPSILON");
	
	HASHTABLEDATA* r;
	char* k;
	bool changed = true;
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		hashtable_insert(ntfirst, k, set_initcb((set_cmpcb)strcmp));
	}
	// generate nonterminal first sets
	while(changed) {
		changed = false;
		for (	hashtable_begin(grammar->rules, &k, &r);
				hashtable_end(grammar->rules);
				hashtable_next(grammar->rules, &k, &r))
		{
			list* l = ((_guilang_rule*)r)->transitions;
			set* firstset = hashtable_get(ntfirst, k);
			int oldsize = firstset->size;
			
			while (l->data != NULL) {
				_guilang_rule_production* p = (_guilang_rule_production*)l->data;
				int i = 0;
				bool inseq = true;
				while (inseq) {
					inseq = false;
					_guilang_token* first = p->production[i];
					switch (first->type) {
						case GUILANG_STRING:
						case GUILANG_NUMBER:
						case GUILANG_KEYWORD:
						case GUILANG_EPSILON:
						case GUILANG_ENDOFINPUT:
							set_add(firstset, first->value);
							break;
						case GUILANG_NONTERMINAL: {
							set* fA = hashtable_get(ntfirst, first->value);
							if (set_has(fA, "EPSILON")) {
								set_difference(temp, fA, eps);
								set_union(firstset, firstset, temp);
								set_clear(temp);
								inseq = true;
							} else {
								set_union(firstset, firstset, fA);
							}
							} break;
						case GUILANG_ENDOFSTRING:
						break;
					}
					i++;
				}
				l = l->next;
			}
			if (oldsize != firstset->size) changed = true;
		}
	}
	
	// generate production first sets
	hashtable* firstsets = hashtable_init(0);
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		list* fl = list_new();
		hashtable_insert(firstsets, k, fl); 
		list* l = ((_guilang_rule*)r)->transitions;
		while (l->data != NULL) {
			set* pfs = set_initcb((set_cmpcb)strcmp);
			list_add(fl, pfs);
			_guilang_rule_production* p = (_guilang_rule_production*)l->data;
			_guilang_token* first = p->production[0];
			
			switch (first->type) {
				case GUILANG_STRING:
				case GUILANG_NUMBER:
				case GUILANG_KEYWORD:
				case GUILANG_EPSILON:
				case GUILANG_ENDOFINPUT:
					set_add(pfs, first->value);
					break;
				case GUILANG_NONTERMINAL: {
					set* fA = hashtable_get(ntfirst, first->value);
					set_union(pfs, pfs, fA);
					} break;
				case GUILANG_ENDOFSTRING:
				break;
			}
			fl = fl->next;
			l = l->next;
		}
	}
	
	// generate nonterminal follow sets
	// initialize each follow set with the empty set
	hashtable* followsets = hashtable_init(0);
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		hashtable_insert(followsets, k, set_initcb((set_cmpcb)strcmp));
	}
	
	changed = true;
	while (changed) {
		changed = false;
		for (	hashtable_begin(grammar->rules, &k, &r);
				hashtable_end(grammar->rules);
				hashtable_next(grammar->rules, &k, &r))
		{
			list* l = ((_guilang_rule*)r)->transitions;
			while (l->data != NULL) {
				_guilang_rule_production* p = (_guilang_rule_production*)l->data;
				for (unsigned int i = 0; i < p->len - 2; i++) {
					_guilang_token* curr = p->production[i];
					if (curr->type == GUILANG_NONTERMINAL) {
						_guilang_token* follow = p->production[i+1];
						set* fs = hashtable_get(followsets, curr->value);
						int osz = fs->size;
						switch (follow->type) {
							case GUILANG_STRING:
							case GUILANG_NUMBER:
							case GUILANG_KEYWORD:
							case GUILANG_EPSILON:
							case GUILANG_ENDOFINPUT:
								set_add(fs, follow->value);
								break;
							case GUILANG_NONTERMINAL: {
								set* fA = hashtable_get(ntfirst, follow->value);
								if (set_has(fA, "EPSILON")) {
									set_union(fs, hashtable_get(followsets, k), fs);
								}
								set_union(fs, fs, fA);
								} break;
							case GUILANG_ENDOFSTRING:
							break;
						}
						if (osz != fs->size) changed = true;
					}
				}
				l = l->next;
			}
		}
	}
	
	// build a new parsing table
	_guilang_parsingtable* ptable = malloc(sizeof(_guilang_parsingtable));
	
	// hashtable of hashtable, indexed but nonterminal, then terminal
	ptable->nti = hashtable_init(0);
	// get set of terminals
	set* terminals = grammar->terminals;
	
	// iterate over nonterminals
	for (	hashtable_begin(grammar->rules, &k, &r);
		hashtable_end(grammar->rules);
		hashtable_next(grammar->rules, &k, &r))
	{
		// build new hashtable for each nonterminal
		hashtable* ti = hashtable_init(0);
		hashtable_insert(ptable->nti, k, ti);
		
		SETDATA* v;
		_guilang_rule_production* rule;
		set* follow = hashtable_get(followsets, k);
				
		// iterate over terminals
		for(set_begin(terminals, &v); set_end(terminals); set_next(terminals, &v)){
			list* flist = hashtable_get(firstsets, k); // list of first sets
			list* prods = ((_guilang_rule*)r)->transitions; // list of transitions
			
			rule = NULL;
			
			// iterate over lists
			while(flist->data != NULL) {

				// get the first set for this transition
				set* f = flist->data;
				
				// if the current terminal is in this first set, set this rule as the current transition
				if (set_has(f, v) ||
					(set_has(f, "EPSILON") && set_has(follow, v))
					) {
					if (rule != NULL) printf("!!!!!!!!!!!ERRORR!!!!!!!\n"); //error here, means ambiguous grammar
					rule = prods->data;
				}
			
				// iter
				prods = prods->next;
				flist = flist->next;
			}
			hashtable_insert(ti, (char*)v, (HASHTABLEDATA*)rule);
		}
	}
	
	set_free(temp);
	printf("NTFIRST:\n");
	hashtable_print(ntfirst, (hashtable_printcb)_guilang_parser_printset);
	printf("FIRST:\n");
	hashtable_print(firstsets, (hashtable_printcb)_guilang_parser_printsetlist);
	printf("FOLLOW:\n");
	hashtable_print(followsets, (hashtable_printcb)_guilang_parser_printset);
	printf("TABLE:\n");
	_guilang_parser_printparsingtable(ptable);
}

int
guilang_parse
(
	guilang_grammar* g,
	_guilang_token** t
)
{
	_guilang_parsingtable* pt = _guilang_parser_initparsingtable(g);
}