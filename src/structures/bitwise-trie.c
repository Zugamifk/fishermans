typedef struct
_S_bittrie_node
{
	struct _S_bittrie_node** child;
} bittrie_node;

bittrie_node*
_bittrie_node_init
(
	void
)
{	
	bittrie_node* node = malloc(sizeof(bittrie_node));
	node->child = calloc(sizeof(bittrie_node*), 2);
	return node;
}

void
_bittrie_node_delete
( bittrie_node* n )
{
	if (n->child[0] != NULL) _bittrie_node_delete(n->child[0]);
	if (n->child[1] != NULL) _bittrie_node_delete(n->child[1]);
	free(n->child);
	free(n);
}

typedef struct
_S_bittrie
{
	bittrie_node* root;
} bittrie;

bittrie*
bittrie_init
(
	void
)
{
	bittrie* trie = malloc(sizeof(bittrie));
	
	trie->root = _bittrie_node_init();
	
	return trie;
}

void
bittrie_insert
(
	bittrie* trie,
	unsigned int* str,
	unsigned int len
)
{
	unsigned int i = 0;
	bittrie_node* pos = trie->root;
	unsigned int b;
	for(;i<len;i++) {
		b = (*str & (1 << i)) >> i;
		if (pos->child[b] == NULL) break;
		else pos = pos->child[b];
	}
	
	for(;i<len;i++) {
		b = (*str & (1 << i)) >> i;
		pos->child[b] = _bittrie_node_init();
		pos = pos->child[b];
	}
}

bool
bittrie_find
(
	bittrie* trie,
	unsigned int* str,
	unsigned int len
)
{
	unsigned int i = 0;
	bittrie_node* pos = trie->root;
	unsigned int b;
	for(;i<len;i++) {
		b = (*str & (1 << i)) >> i;
		if (pos->child[b] == NULL) return false;
		else pos = pos->child[b];
	}
	
	return true;
}

void
bittrie_delete
(
	bittrie* trie
)
{
	_bittrie_node_delete(trie->root);
	free(trie);
}

void
bittrie_test
(
	void
)
{
	bittrie* trie = bittrie_init();
	unsigned int 
		a = 0xC0FFEE,
		b = 0xB00B1E5,
		c = 0x31337;
		
	bittrie_insert(trie, &a, 4*6);
	bittrie_insert(trie, &b, 4*7);
	
	printf("A: %d\tB: %d\tC: %d\n", bittrie_find(trie, &a, 4*6), bittrie_find(trie, &b, 4*7), bittrie_find(trie, &c, 4*5));
	bittrie_delete(trie);
}