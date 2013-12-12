// ================================================
// X-FAST TRIE
// ================================================

#define XFASTTRIEDATA void
#define XFASTBYTE char
#define XFASTBYTESIZE CHAR_BIT
#define XFAST_LSSROOT(TRIE) TRIE->lss[0]
#define XFAST_LSSLEAF(TRIE) TRIE->lss[TRIE->depth-1]

#define XFAST_DEBUG

typedef enum 
_xfast_nodelink_e
{
	XFAST_LINK_NULL,
	XFAST_LINK_CHILD,
	XFAST_LINK_DESCENDANT,
	XFAST_LINK_ADJACENT
} _xfast_nodelinktype_t;

const char* _xfast_nodelinkstrings[] = {
	"NULL",
	"CHILD",
	"DESCENDANT",
	"ADJACENT"
};

typedef enum
_xfast_direction_e
{
	XFAST_LEFT,
	XFAST_RIGHT,
	XFAST_SUCC,
	XFAST_PRED
} _xfast_direction_t;

typedef struct
_S_xfast_node
{
	XFASTBYTE* label;
	_xfast_nodelinktype_t left;
	_xfast_nodelinktype_t right;
	struct _S_xfast_node** links;
} xfast_node;

typedef struct
_S_xfasttrie
{
	xfast_node* root;
	xfast_node* inf;
	xfast_node* ninf;
	hashtable** lss;
	hashtable* data;
	unsigned int depth;
	unsigned int keylen;
} xfasttrie;

// TRIE NODES
// ================================================
xfast_node*
_xfast_node_init
(
	void
)
{	
	xfast_node* node = malloc(sizeof(xfast_node));
	node->label = NULL;
	node->left = XFAST_LINK_NULL;
	node->right = XFAST_LINK_NULL;
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

void
_xfast_printbyte
( XFASTBYTE b )
{
	printf("%d%d%d%d%d%d%d%d",
		BT_BIT(b,7),
		BT_BIT(b,6),
		BT_BIT(b,5),
		BT_BIT(b,4),
		BT_BIT(b,3),
		BT_BIT(b,2),
		BT_BIT(b,1),
		BT_BIT(b,0) );
}

void
_xfast_printbytestring
( XFASTBYTE* str, unsigned int len )
{
	for (int i = 0; i < len; i++) {
		printf(" ");
		_xfast_printbyte(str[i]);
	}
	printf("\n");
}

void
xfast_node_print
( xfast_node* n, unsigned int len )
{
	printf("NODE [%s] \'", n->label);
	_xfast_printbytestring(n->label, len);
	printf("\'\n");
	xfast_node *left = n->links[0];
	xfast_node *right = n->links[1];
	printf("\tLEFT [%s]: ", _xfast_nodelinkstrings[n->left]);
	if (left == NULL) printf("NULL\n");
	else _xfast_printbytestring(left->label, len);
	printf("\tRIGHT [%s]: ", _xfast_nodelinkstrings[n->left]);
	if (right == NULL) printf("NULL\n");
	else _xfast_printbytestring(right->label, len);
}

// LSS
// ================================================

void
xfast_lss_addnode
(
	hashtable* ls,
	xfast_node* node
)
{
	hashtable_insert(ls, node->label, node);
}

void
xfast_lss_init
(
	xfasttrie* trie,
	unsigned int depth
)
{
	trie->lss = calloc(sizeof(hashtable*), depth);
	for (unsigned int i = 0; i < depth; i++) {
		trie->lss[i] = hashtable_init(1);
	}
	xfast_lss_addnode(XFAST_LSSROOT(trie), trie->root);
	xfast_lss_addnode(XFAST_LSSLEAF(trie), trie->inf);
	xfast_lss_addnode(XFAST_LSSLEAF(trie), trie->ninf);
}

// Linked list
// ================================================
void
xfast_list_insert
(
	xfast_node* value,
	xfast_node* pred,
	xfast_node* succ
)
{
	pred->right = XFAST_LINK_ADJACENT; 
	pred->links[1] = value;
	value->left = XFAST_LINK_ADJACENT;
	value->links[0] = pred;
	value->right = XFAST_LINK_ADJACENT;
	value->links[1] = succ;
	succ->left = XFAST_LINK_ADJACENT;
	succ->links[0] = value;	
}

// X-FAST TRIE
// ================================================
xfasttrie*
xfasttrie_init
(
	unsigned int depth
)
{
	xfasttrie* trie = malloc(sizeof(xfasttrie));
	
	trie->root = _xfast_node_init();
	trie->inf = _xfast_node_init();
	trie->ninf = _xfast_node_init();
	trie->depth = depth;
	trie->keylen = ((depth-1)/XFASTBYTESIZE) + 1;
	
	trie->root->label = calloc(1, depth/XFASTBYTESIZE);
	trie->root->left  = XFAST_LINK_NULL;
	trie->root->right  = XFAST_LINK_NULL;
	trie->root->links[0] = trie->ninf;
	trie->root->links[1] = trie->inf;
	
	trie->inf->label = calloc(1, depth/XFASTBYTESIZE);
	strcpy(trie->inf->label, "INF");
	trie->inf->left = XFAST_LINK_ADJACENT;
	trie->inf->right = XFAST_LINK_ADJACENT;
	trie->inf->links[0] = trie->ninf;
	trie->inf->links[1] = trie->inf;
	
	trie->ninf->label = calloc(1, depth/XFASTBYTESIZE);
	strcpy(trie->ninf->label, "NINF");
	trie->ninf->left = XFAST_LINK_ADJACENT;
	trie->ninf->right = XFAST_LINK_ADJACENT;
	trie->ninf->links[0] = trie->ninf;
	trie->ninf->links[1] = trie->inf;
	
	xfast_lss_init(trie, depth);
	
	return trie;
}

unsigned int
xfasttrie_getbit
(
	XFASTBYTE* key,
	unsigned int pos
)
{
	unsigned int bytepos = pos/XFASTBYTESIZE;
	unsigned int bitpos = pos%XFASTBYTESIZE;
	return BT_BIT(key[bytepos], bitpos);
}
	
void
xfasttrie_getkeyprefix
(
	XFASTBYTE* prefix,
	XFASTBYTE* key,
	unsigned int pos,
	unsigned int len
)
{
	int i = 0;
	if (pos > 0) {
		unsigned int bytepos = pos/XFASTBYTESIZE;
		unsigned int bitpos = XFASTBYTESIZE-pos%XFASTBYTESIZE;
		for (;i < bytepos; i++) {
			prefix[i] = key[i];
		}
		prefix[bytepos] = (key[bytepos]>>bitpos)<<bitpos;
	}
	for (i++;i<len;i++) {
		prefix[i] = 0;
	}
}

XFASTTRIEDATA*
xfasttrie_get
(
	xfasttrie* trie,
	XFASTBYTE* key
)
{
	return hashtable_get(trie->data, key);
}

xfast_node*
_xfasttrie_lookup
(
	xfasttrie* trie,
	unsigned int level,
	XFASTBYTE* key
)
{
	return hashtable_get(trie->lss[level], (char*)key);
}

xfast_node*
_xfasttrie_getancestor
(
	xfasttrie* trie,
	XFASTBYTE* key,
	unsigned int* index
)
{
	int pos = trie->depth/2;
	int cursor = pos;
	XFASTBYTE prefix[trie->keylen];
	xfast_node* ancestor = trie->root;
	while (cursor>0) {
		cursor>>=1;
		xfasttrie_getkeyprefix(prefix, key, pos, trie->keylen);
		xfast_node* node = _xfasttrie_lookup(trie, pos, prefix);
		if (node == NULL) {
			pos -= cursor;
		} else {
			ancestor = node;
			pos += cursor;
		}
	}
	if (index != NULL) *index = pos;
	return ancestor;
}

xfast_node*
_xfasttrie_adj
(
	xfasttrie* trie,
	XFASTBYTE* key,
	_xfast_direction_t dir
)
{

	xfast_node* ancestor = _xfasttrie_getancestor(trie, key, NULL);
	xfast_node* left = ancestor->links[0];
	xfast_node* right = ancestor->links[1];
	
	switch(dir) {
		case XFAST_SUCC: {
			switch (ancestor->left) {
				case XFAST_LINK_NULL: return trie->inf;
				case XFAST_LINK_CHILD: return right->links[1];
				case XFAST_LINK_DESCENDANT: return left;
				default: break;
			}
		} break;
		case XFAST_PRED: {
			switch (ancestor->right) {
				case XFAST_LINK_NULL: return trie->ninf;
				case XFAST_LINK_CHILD: return right->links[0];
				case XFAST_LINK_DESCENDANT: return right;
				default: break;
			}
		} break;
		default: break;
	}
	printf("THIS SHOULDN'T HAPPEN! xfast_node* _xfasttrie_adj( xfasttrie* trie, XFASTBYTE* key, _xfast_direction_t dir ) in xfast.c!");
	return NULL;
}

xfast_node*
xfast_succ
(
	xfasttrie* trie,
	XFASTBYTE* key
)
{
	return _xfasttrie_adj(trie, key, XFAST_SUCC);
}

xfast_node*
xfast_pred
(
	xfasttrie* trie,
	XFASTBYTE* key
)
{
	return _xfasttrie_adj(trie, key, XFAST_PRED);
}

void
xfasttrie_insert
(
	xfasttrie* trie,
	XFASTBYTE* key,
	XFASTTRIEDATA* value
)
{
	// its already there
	if (xfasttrie_get(trie, key)) return;
	
	// create new node
	xfast_node* new = _xfast_node_init(); 
	
	// Get predecessor and successor nodes
	xfast_node* pred = xfast_pred(trie, key);
	xfast_node* succ = xfast_succ(trie, key);
	
	// Add new node to linked list at bottom tier
	xfast_list_insert(new, pred, succ);
	xfast_node_print(new, 8);
	
	// Add value to data table and key to trie leaves
	hashtable_insert(XFAST_LSSLEAF(trie), key, new);
	hashtable_insert(trie->data, key, value);
	
	// allocate space for prefix
	XFASTBYTE prefix[trie->keylen];
	unsigned int i, bit;
	xfast_node* ancestor = _xfasttrie_getancestor(trie, key, &i);
	xfast_node* child;
	while(i < trie->depth-1) {
		child = _xfast_node_init();
		bit = xfasttrie_getbit(key, i);
		ancestor->links[bit] = child;
		child->links[0] = new;
		child->links[1] = new;
		xfasttrie_getkeyprefix(prefix, key, i, trie->depth);
		hashtable_insert(trie->lss[i+1], prefix, child);
		ancestor = child;
		i++;
	}
	
}

void
xfasttrie_remove
(
	xfasttrie* trie,
	XFASTBYTE* key
)
{
	xfast_node* node = hashtable_get(XFAST_LSSLEAF(trie), key);
	xfast_node* pred = node->links[0];
	xfast_node* succ = node->links[1];
	pred->links[1] = succ;
	succ->links[0] = pred;
	
	hashtable_remove(XFAST_LSSLEAF(trie), key);
	hashtable_remove(trie->data, key);
	
	XFASTBYTE prefix[trie->keylen];
	int i = trie->depth-1;
	unsigned int bit;
	xfast_node* cursor;
	while (i>0) {
		xfasttrie_getkeyprefix(prefix, key, i, trie->depth);
		cursor = hashtable_get(trie->lss[i], prefix);
		bit = xfasttrie_getbit(key, i);
		if (cursor->links[!bit] == node) {
			hashtable_remove(trie->lss[i], prefix);
			_xfast_node_delete(cursor);
		} else {
			if (bit == 0)
				cursor->links[bit] = succ;
			else
				cursor->links[bit] = pred;
			break;
		}
		
	}
	_xfast_node_delete(node);
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
	char keys[3][16] = {"coffee", "boobies", "elite"};
	printf("%d\n", sizeof(unsigned int));
	for (int j = 1; j<16;j++) {
		xfasttrie* trie = xfasttrie_init(128);
		tick();
		for (int i = 1<<j; i>0;i--) {
				
			xfasttrie_insert(trie, keys[0], (void*)(4*6));
			xfasttrie_insert(trie, keys[1], (void*)(4*7));
			
			//xfasttrie_successor(trie, c);
			
			//printf("A: %d\tB: %d\tC: %d\n", xfasttrie_find(trie, &a, 4*6), xfasttrie_find(trie, &b, 4*7), xfasttrie_find(trie, &c, 4*5));
		}
		tock();
		xfasttrie_delete(trie);
	}
}