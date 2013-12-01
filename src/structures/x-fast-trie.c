// ================================================
// X-FAST TRIE
// ================================================

// TRIE NODES
// ================================================
typedef struct
_S_xfast_node
{
	unsigned int* value;
	struct _S_xfast_node** links;
} xfast_node;

xfast_node*
_xfast_node_init
(
	void
)
{	
	xfast_node* node = malloc(sizeof(xfast_node));
	node->links = calloc(sizeof(xfast_node*), 2);
	return node;
}

void
_xfast_node_delete
( xfast_node* n )
{
	if (n->links[0] != NULL) _xfast_node_delete(n->links[0]);
	if (n->links[1] != NULL) _xfast_node_delete(n->links[1]);
	free(n->links);
	free(n);
}

// LEVEL SEARCH STRUCTURE TABLE
// ================================================
#define XFASTLGWORD 4
#define XFASTWORD (1<<XFASTLGWORD)
#define XFASTBINPOW 4
#define XFASTHASHP1 ((1<<XFASTWORD)-1)	// must be odd < 2^8 = 256
#define XFASTHASHT1P2 (1<<XFASTLGWORD)-1	// must be < 2^(8-4) = 16
#define XFASTHASHT2P2 1
#define XFASTHASH1(x) (unsigned)(XFASTHASHP1 * x + XFASTHASHT1P2)>>(XFASTWORD-XFASTBINPOW)
#define XFASTHASH2(x) (unsigned)(XFASTHASHP1 * x + XFASTHASHT2P2)>>(XFASTWORD-XFASTBINPOW*2)

typedef struct
_S_xfast_hashtable2
{
	xfast_node** table;
} xfast_hashtable2;

typedef struct
_S_xfast_hashtable
{
	xfast_hashtable2** subtable;
} xfast_hashtable;

xfast_hashtable*
_xfast_hashtable_init
( void )
{ 
	xfast_hashtable* ht = malloc(sizeof(xfast_hashtable));
	ht->subtable = calloc(sizeof(xfast_hashtable2*), 1<<XFASTBINPOW);
	return ht;
}

xfast_hashtable2*
_xfast_hashtable2_init
( void )
{ 
	xfast_hashtable2* st = malloc(sizeof(xfast_hashtable2));
	st->table = calloc(sizeof(xfast_hashtable2*), 1<<(XFASTBINPOW*2));
	return st;
}

void
_xfast_hashtable_insert
( xfast_hashtable* ht, unsigned int key, xfast_node* value) {
	unsigned int i = XFASTHASH1(key);
	unsigned int j = XFASTHASH2(key);
	xfast_hashtable2* st = ht->subtable[i];
	if (st == NULL) st = _xfast_hashtable2_init();
	st->table[j] = value;
}

bool
_xfast_hashtable_find
( xfast_hashtable* ht,  unsigned int key) {
	unsigned int i = XFASTHASH1(key);
	unsigned int j = XFASTHASH2(key);
	printf("i:%d j:%d\n", i, j);
	xfast_hashtable2* st = ht->subtable[i];
	return !(st == NULL || st->table[j] == NULL);
}

xfast_node*
_xfast_hashtable_get
( xfast_hashtable* ht,  unsigned int key) {
	unsigned int i = XFASTHASH1(key);
	unsigned int j = XFASTHASH2(key);
	printf("2i:%d j:%d\n", i, j);
	xfast_hashtable2* st = ht->subtable[i];
	if (st == NULL) return NULL;
	else return st->table[j];
}

// X-FAST TRIE
// ================================================
typedef struct
_S_xfasttrie
{
	xfast_node* root;
	xfast_hashtable** lss;
} xfasttrie;

xfasttrie*
xfasttrie_init
(
	void
)
{
	xfasttrie* trie = malloc(sizeof(xfasttrie));
	
	trie->root = _xfast_node_init();
	trie->lss = calloc(sizeof(xfast_hashtable), XFASTWORD);
	for (int i =0; i<XFASTWORD;i++) {
		trie->lss[i] = _xfast_hashtable_init();
	}
	
	return trie;
}

void
xfasttrie_insert
(
	xfasttrie* trie,
	unsigned int* str,
	unsigned int len
)
{
	unsigned int i = 0;
	xfast_node* pos = trie->root;
	unsigned int b;
	for(;i<len;i++) {
		b = (*str & (1 << i)) >> i;
		if (pos->links[b] == NULL) break;
		else pos = pos->links[b];
	}
	
	for(;i<len;i++) {
		b = (*str & (1 << i)) >> i;
		pos->links[b] = _xfast_node_init();
		pos = pos->links[b];
	}
}

bool
xfasttrie_find
(
	xfasttrie* trie,
	unsigned int str
)
{
	return _xfast_hashtable_find(trie->lss[0], str);
}

xfast_node*
xfasttrie_successor
(
	xfasttrie* trie,
	unsigned int str
)
{
	unsigned int prefix, i, h = XFASTWORD>>1;
	xfast_node* ancestor = NULL;
	for(int e = 2; e <= XFASTLGWORD; e++) {
		i = XFASTWORD-h;
		prefix = str>>i;
		if (_xfast_hashtable_find(trie->lss[i], prefix)) {
			ancestor = _xfast_hashtable_get(trie->lss[i], prefix);
			h = h + (XFASTWORD>>e);
		} else {
			h = h - (XFASTWORD>>e);
		}
		printf("p:%d i:%d h:%d e:%d\n", prefix, i, h, e);
	}
	if (_xfast_hashtable_find(trie->lss[i], prefix)) 
		ancestor = _xfast_hashtable_get(trie->lss[i], prefix);
	
	if (ancestor->links[0]->value == NULL) {
		return ancestor->links[1];
	} else return ancestor->links[0]->links[1];
}

void
xfasttrie_delete
(
	xfasttrie* trie
)
{
	_xfast_node_delete(trie->root); 
	free(trie);
}

void
xfasttrie_test
(
	void
)
{
	printf("%d\n", sizeof(unsigned int));
	for (int j = 1; j<16;j++) {
		xfasttrie* trie = xfasttrie_init();
		tick();
		for (int i = 1<<j; i>0;i--) {
			unsigned int 
				a = 0xC0FFEE +i,
				b = 0xB00B1E5 +i,
				c = 0x31337 +i;
				
			xfasttrie_insert(trie, &a, 4*6);
			xfasttrie_insert(trie, &b, 4*7);
			
			xfasttrie_successor(trie, c);
			
			//printf("A: %d\tB: %d\tC: %d\n", xfasttrie_find(trie, &a, 4*6), xfasttrie_find(trie, &b, 4*7), xfasttrie_find(trie, &c, 4*5));
		}
		tock();
		xfasttrie_delete(trie);
	}
}