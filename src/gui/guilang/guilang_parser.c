void
_guilang_parser_printset
(
	void* d
)
{
	set_print((set*)d, (set_printcb)_guilang_printstr);
}

typedef struct
_S_guilang_parsingtable
{
	hashtable* table;
} _guilang_parsingtable;

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
	// generate first sets
	while(changed) {
		hashtable_print(ntfirst, (hashtable_printcb)_guilang_parser_printset);
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
									set_intersection(temp, fA, eps);
									set_union(firstset, firstset, temp);
									set_clear(temp);
									inseq = true;
								} else {
									set_union(firstset, firstset, fA);
								}
								} break;
							case GUILANG_ENDOFSTRING:
								set_add(firstset, "EPSILON");
								break;
						}
						i++;
					}
					l = l->next;
				}
				if (oldsize != firstset->size) changed = true;
			}
	}
	set_free(temp);
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