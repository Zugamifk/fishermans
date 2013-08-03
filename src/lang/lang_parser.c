// ==================================================
// LANG PARSER : 
// Builds a parsing table with an pre-constructed grammar.
// Does checks for ambiguities in the grammar.
// ==================================================

// DEFINITIONS
// ==================================================
typedef struct
_S_lang_parsingtable
{
	hashtable* nti;
	lang_grammar* grammar;
} _lang_parsingtable;

// PRINT FUNCTIONS
// ==================================================

void
_lang_parser_shtstr
(
	char* w
)
{
	int l = strlen(w);
	if (l < 6) {
		printf("%s", w);
	} else {
		int m = l/2;
		printf("%c%c%c%c%c", w[0], w[1], w[2], w[m], w[l-1]);
	}
}

void
_lang_parser_printset
(
	void* d
)
{
	set_print((set*)d, (set_printcb)_lang_printstr);
}

void
_lang_parser_printsetlist
(
	void* d
)
{
	list* l = (list*)d;
	printf("\n");
	while(l->data!=NULL) {
	printf("\t\t");
		set_print((set*)l->data, (set_printcb)_lang_printstr);
		l = l->next;
	}
}

void
_lang_parser_printparsingtable
(
	_lang_parsingtable* t
)
{
	set* terminals = t->grammar->terminals;
	SETDATA* v;
	for (set_begin(terminals, &v); set_end(terminals); set_next(terminals, &v)) {
		char* term = (char*)v;
		_lang_parser_shtstr(term);
		printf("\t");
	}
	printf("%d\n", terminals->size);
	HASHTABLEDATA* ti;
	char* nt;
	for(hashtable_begin(t->nti, &nt, &ti);
		hashtable_end(t->nti);
		hashtable_next(t->nti, &nt, &ti))
	{
		_lang_parser_shtstr(nt);
		HASHTABLEDATA* predict;
		char* tt;
		for(hashtable_begin((hashtable*)ti, &tt, &predict);
			hashtable_end((hashtable*)ti);
			hashtable_next((hashtable*)ti, &tt, &predict))
		{
			_lang_rule_production* rule = (_lang_rule_production*)predict;
			printf("\t[");
			if (rule != NULL) {
				char* str = rule->production[0]->value;
				_lang_parser_shtstr(str);
			}
			printf("]");
		}
		printf("\n");
	}
}

void
_lang_parser_freelist
(
	list* d
)
{
	list_deepdelete(d, (list_deletedatacb)set_free);
}

void
_lang_parser_freeparsingtableentry
(
	hashtable* t
)
{
	hashtable_deepfree(t, NULL);
}

void
_lang_parser_freeparsingtable
(
	_lang_parsingtable* p
)
{
	hashtable_deepfree(p->nti, (hashtable_freecb)_lang_parser_freeparsingtableentry);
}

// PARSING TABLE
/*
	Builds 4 hashtables:
	1.	First(A) for each nonterminal A
	2.	First(B) for each production A -> B
	3.	Follow(A) for each nonterminal A
	4.	Predict(A, a) for each nonterminal A and each terminal a
	The predict table is stored in the parsing table and used for parsing and
	any other necessary analysis.
*/
// ===========================================================================*/
/**/	_lang_parsingtable*				//	A fully built parsing table
/**/	_lang_parser_initparsingtable	//
/*============================================================================*/
(
	lang_grammar* grammar	// The grammar to build the parsing table from
)
/*============================================================================*/
{	
	// local set for operations
	set* temp = set_init();
	
	// set containing only epsilon
	set* eps = set_init();
	set_add(eps, "EPSILON");
	
	// values for iterating over tables
	HASHTABLEDATA* r;
	char* k;
	
	// Initialize First(A) table
	hashtable* ntfirst = hashtable_init(0);
	
	// Initialize each First(A) with an empty set 
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		hashtable_insert(ntfirst, k, set_initcb((set_cmpcb)strcmp));
	}

	// Loop over steps until no changes occur
	bool changed = true;
	while(changed) {
		// Reset sentinal
		changed = false;

		// Iterate over nonterminals
		// K := Nonterminal
		// R := List of productions
		for (	hashtable_begin(grammar->rules, &k, &r);
				hashtable_end(grammar->rules);
				hashtable_next(grammar->rules, &k, &r))
		{
			// Pull out list of productions
			list* l = ((_lang_rule*)r)->transitions;
			
			// Get First(K) = {}
			set* firstset = hashtable_get(ntfirst, k);
			
			// Variable to track changes in sets
			int oldsize = firstset->size;
			
			// Iterate over productions
			while (l->data != NULL) {
			
				// Pull out production
				_lang_rule_production* p = (_lang_rule_production*)l->data;
				
				// Iterate over production terms
				int i = 0;
				bool inseq = true;
				while (inseq) {
					inseq = false;
					
					// get term
					_lang_token* term = p->production[i];
					
					// Act based on token type
					switch (term->type) {
						// Add terminals to the first set
						case LANG_STRING:
						case LANG_NUMBER:
						case LANG_KEYWORD:
						case LANG_EPSILON:
						case LANG_ENDOFINPUT:
							set_add(firstset, term->value);
							break;
						// Merge first sets of nonterminals
						case LANG_NONTERMINAL: {
							set* fA = hashtable_get(ntfirst, term->value);
							// If it has an epsilon, skip the epsilon and add the first set of the next term as well
							if (set_has(fA, "EPSILON")) {
								set_copy(temp, fA);
								set_remove(temp, "EPSILON");
								set_union(firstset, firstset, temp);
								set_clear(temp);
								inseq = true;
							} else {
							// Otherwise add all terms to the first set
								set_union(firstset, firstset, fA);
							}
							} break;
						// The end of string is effectively empty and counts as an epsilon
						case LANG_ENDOFSTRING:
						//	set_add(firstset, "EPSILON");
							break;
					}
					i++;
				}
				l = l->next;
			}

			// Continue if changed
			if (oldsize != firstset->size) changed = true;
		}
		
	}

	/* -----------------------------------------------------------------------*/
	// generate production first sets
	
	// Initialize First(B) table
	hashtable* firstsets = hashtable_init(0);
	
	// iterate over rules
	// K := nonterminals
	// R := list of productions
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		// Initialize new list to hold first sets
		list* fl = list_new();
		hashtable_insert(firstsets, k, fl);
		
		// Pull out productions and iterate over them
		list* l = ((_lang_rule*)r)->transitions;
		while (l->data != NULL) {
		
			// Initialize First(B)
			set* pfs = set_initcb((set_cmpcb)strcmp);
			list_add(fl, pfs);
			
			// Get the first term of the production
			_lang_rule_production* p = (_lang_rule_production*)l->data;
			_lang_token* first = p->production[0];
			
			// Add to First(B) based on type
			switch (first->type) {
				// Add terminal as items
				case LANG_STRING:
				case LANG_NUMBER:
				case LANG_KEYWORD:
				case LANG_EPSILON:
				case LANG_ENDOFINPUT:
					set_add(pfs, first->value);
					break;
				// First(B) = First(B) U First(p)
				case LANG_NONTERMINAL: {
					set* fA = hashtable_get(ntfirst, first->value);
					set_union(pfs, pfs, fA);
					} break;
				case LANG_ENDOFSTRING:
				break;
			}
			fl = fl->next;
			l = l->next;
		}
	}

	/* -----------------------------------------------------------------------*/
	// generate follow sets

	// initialize each follow set with the empty set
	hashtable* followsets = hashtable_init(0);
	for (	hashtable_begin(grammar->rules, &k, &r);
			hashtable_end(grammar->rules);
			hashtable_next(grammar->rules, &k, &r))
	{
		hashtable_insert(followsets, k, set_initcb((set_cmpcb)strcmp));
	}
	
	set_add(hashtable_get(followsets, grammar->startsymbol->value), "EOI");
	
	// Loop until unchanged
	changed = true;
	while (changed) {
		changed = false;
		
		// Iterate over nonterminals
		// K := Nonterminal
		// R := list of productions
		for (	hashtable_begin(grammar->rules, &k, &r);
				hashtable_end(grammar->rules);
				hashtable_next(grammar->rules, &k, &r))
		{
			// Pull out productions and iterate
			list* l = ((_lang_rule*)r)->transitions;
			while (l->data != NULL) {
			
				// Get production and iterate
				// -1: ignore ENDOFSTRING
				// -1: last term has no Follow
				_lang_rule_production* p = (_lang_rule_production*)l->data;
				for (unsigned int i = 0; i < p->len - 1; i++) {
					// If the current term is a nonterminal, find a follow set
					_lang_token* curr = p->production[i];
					if (curr->type == LANG_NONTERMINAL) {
						// get the follow set for the current term
						set* fs = hashtable_get(followsets, curr->value);
						
						// sentinal for changes
						int osz = fs->size;
							
						int seqi = i+1;
						for(;seqi<p->len; seqi++) {
							// get the next term
							_lang_token* follow = p->production[seqi];
							
							// act based on next term type
							switch (follow->type) {
								// Add terminals to current's follow set
								case LANG_STRING:
								case LANG_NUMBER:
								case LANG_KEYWORD:
								case LANG_ENDOFINPUT:
									set_add(fs, follow->value);
									goto nextprod;
								// Nonterminals are complicated
								case LANG_NONTERMINAL: {
									// Get the first set of the next term
									set* fA = hashtable_get(ntfirst, follow->value);
									// If fA has an epsilon, Follow(curr) += Follow(k), where k is the current rule's NONTERMINAL
									// This may be incorrect!
									
									if (set_has(fA, "EPSILON")) {
										set_copy(temp, fA);
										set_remove(temp, "EPSILON");
										set_union(fs, fs, temp);
										set_clear(temp);
									} else {
										// Add fA to Follow(curr)
										set_union(fs, fs, fA);
										goto nextprod;
									}
									} break;
								case LANG_EPSILON:
								goto nextprod;
								break;
								case LANG_ENDOFSTRING: {
									set* fPROD = hashtable_get(followsets, k);
									set_union(fs, fs, fPROD);
									goto nextprod;
								} break;
							}
						}
						nextprod:
						// Check sentinal
						if (osz != fs->size) changed = true;
					}
				}
				l = l->next;
			}
		}
	}

	/* -----------------------------------------------------------------------*/
	// Build a new parsing table

	// Allocate space
	_lang_parsingtable* ptable = malloc(sizeof(_lang_parsingtable));
	ptable->grammar = grammar;
	
	// hashtable of hashtables, indexed by [nonterminal, terminal]
	ptable->nti = hashtable_init(0);
	// get set of terminals
	set* terminals = grammar->terminals;
	
	// Iterate over nonterminals
	// K := nonterminals
	// R := list of productions
	for (	hashtable_begin(grammar->rules, &k, &r);
		hashtable_end(grammar->rules);
		hashtable_next(grammar->rules, &k, &r))
	{
		// Build new hashtable for each nonterminal
		hashtable* ti = hashtable_init(0);
		hashtable_insert(ptable->nti, k, ti);
		
		// variable to hold terminal
		SETDATA* v;
		
		// pointer for predict set
		_lang_rule_production* rule;
		
		// get follow set for current nonterminal
		set* follow = hashtable_get(followsets, k);
				
		// iterate over terminals
		// v := terminal
		for(set_begin(terminals, &v); set_end(terminals); set_next(terminals, &v)){
			list* flist = hashtable_get(firstsets, k); // list of first sets
			list* prods = ((_lang_rule*)r)->transitions; // list of transitions
			
			// Reset current production
			rule = NULL;
			
			// iterate over lists
			while(flist->data != NULL) {

				// get the first set for this transition
				set* f = flist->data;
				
				// if the current terminal is in this first set, set this rule as the current transition
				if (set_has(f, v) ||
					// Or if this first set contains epsilon, check the follow set
					(set_has(f, "EPSILON") && set_has(follow, v))
					) {
					if (rule != NULL) printf("!!!!!!!!!!!ERRORR!!!!!!!\n"); //error here, means ambiguous grammar
					rule = prods->data;
				}
			
				// iter
				prods = prods->next;
				flist = flist->next;
			}
			
			// Insert Predict(k, v) in the parsing table
			hashtable_insert(ti, (char*)v, (HASHTABLEDATA*)rule);
		}
	}

	// Print test stuff
	set_free(temp);
	#ifdef LANG_PRINTNTFIRST
	printf("NTFIRST:\n");
	hashtable_print(ntfirst, (hashtable_printcb)_lang_parser_printset);
	#endif
	
	#ifdef LANG_PRINTFIRST
	printf("FIRST:\n");
	hashtable_print(firstsets, (hashtable_printcb)_lang_parser_printsetlist);
	#endif
	
	#ifdef LANG_PRINTFOLLOW
	printf("FOLLOW:\n");
	hashtable_print(followsets, (hashtable_printcb)_lang_parser_printset);
	#endif
	
	#ifdef LANG_PRINTPARSINGTABLE
	printf("TABLE:\n");
	_lang_parser_printparsingtable(ptable);
	#endif

	hashtable_deepfree(ntfirst, (hashtable_freecb)set_free);
	hashtable_deepfree(firstsets, (hashtable_freecb)_lang_parser_freelist);
	hashtable_deepfree(followsets, (hashtable_freecb)set_free);
	
	return ptable;
}
/*============================================================================*/
// Parsing table operations

// Generic error, takes two strings, probably token mismatch related
void
_lang_parser_error
(
	errorlog* log,
	const char* message,
	const char* a1,
	const char* a2
)
{
	char errorstring[LANG_LINELEN]; 
	sprintf(errorstring, message, a1, a2);
	errorlog_logdef(log, "LANG PARSER", errorstring);
}

// General token mismatch error
void
_lang_parser_mismatch
(
	errorlog* log,
	const char* a1,
	const char* a2
)
{
	char errorstring[LANG_LINELEN]; 
	sprintf(errorstring, "Token mismatch: expected [ %s ], got [ %s ]", a1, a2);
	errorlog_logdef(log, "LANG PARSER", errorstring);
}

// Get a production from the parsing table
_lang_rule_production*
_lang_parser_getproduction
(
	_lang_parsingtable* pt,	// Parsing table
	_lang_token* nt,			// Nonterminal
	_lang_token* t			// Terminal
)
{
	// Get the nonterminal entry
	hashtable* ti = hashtable_get(pt->nti, nt->value);
	// Get keywords and numbers by a generic value, otherwise use the given terminal's value
	if (t->type == LANG_KEYWORD) {
		return hashtable_get(ti, LANG_GENERICWORD);
	} else 
	if (t->type == LANG_NUMBER) {
		return hashtable_get(ti, LANG_GENERICNUM);
	} else return hashtable_get(ti, t->value);
}

// PARSER
/*	Standard LL(1) Parser
	Builds a parsing table and initializes a stack with the start and end
	symbols. The loop iterates as long as there are tokens on the stack and pops
	one value every iteration. 
	Every loop, one of two things happens:
	1.	If there is a terminal on top of the stack, compare it to the current 
		input token. If their types and values match, consume both.
	2.	If there is a nonterminal on the stack, reference the parsing table with
		the nonterminal and the current input token. Push all of the tokens from
		ther table's entry onto the stack.
	In either case, if the required conditions fail, a syntax error is thrown.
*/
/*============================================================================*/
/**/	int				// Success indicator to pass onto code translator
/**/	lang_parse	// 
/*============================================================================*/
(
	lang_grammar* g,		// The grammar containing all productions
	_lang_token** t,		// Input token stream
	errorlog* log			// Error log
)
/*============================================================================*/
{
	// Build a parsing taken for the given grammar
	_lang_parsingtable* pt = _lang_parser_initparsingtable(g);

	// Initialize the end of input token for the stack
	_lang_token* eoi = _lang_inittoken(LANG_ENDOFINPUT, "$$");
	
	// Initialize the stack, push the start and end tokens onto it
	stack* s = stack_init(0); 
	stack_push(s, eoi);
	stack_push(s, g->startsymbol);
	
	// Current stack token
	_lang_token* top;
	
	// Current input token
	_lang_token* in;
	_lang_token** cursor = t;
	
	// Loop until stack is empty
	while (!stack_isempty(s)) {
	
		// Pop the stack and get current input token
		top = (_lang_token*)stack_pop(s);
		in = *cursor;
		
		// Act depending on stack value
		switch (top->type) {
			// Nonterminal: get a production and push it onto the stack
			case LANG_NONTERMINAL: {
				// Get a production from the table
				_lang_rule_production* prod = _lang_parser_getproduction(pt, top, in);
				// If no production, syntax error
				if (prod == NULL) {
					_lang_parser_error(log, "No production in [ %s ] for terminal [ %s ]", top->value, in->value);
					break;
				};
				// Otherwise push each token in the production onto the stack
				for (int i = prod->len-2; i >= 0; i--) {
					stack_push(s, prod->production[i]);
				}
			} break;
			// String terminal: match token type and value, otherwise syntax error
			case LANG_STRING: {
				if (in->type == LANG_STRING) 
				{
					if (strcmp(in->value, top->value) == 0) {
						cursor++;
					} else {
						_lang_parser_error(log, "String token mismatch: expected [ %s ], got [ %s ]", top->value, in->value);
					}
				} else {
					_lang_parser_mismatch(log,  _lang_tokentypestrings[top->type], _lang_tokentypestrings[in->type]);
				}
			} break;
			// Other terminals: match ttype, otehrwise syntax error
			case LANG_NUMBER: 
			case LANG_KEYWORD: 
			case LANG_ENDOFINPUT: {
				if (in->type == top->type) {
					cursor++;
				} else {
					_lang_parser_mismatch(log,  _lang_tokentypestrings[top->type], _lang_tokentypestrings[in->type]);
				}
			} break;
			case LANG_ENDOFSTRING:
			case LANG_EPSILON:
			break;
		}

	}
	_lang_parser_freeparsingtable(pt);
	stack_free(s);
	_lang_deletetoken(eoi);
	return 0;
}