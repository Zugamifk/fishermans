#define  BINTREEDATA void
#define BINTREENULL NULL
#define BINTREEFREEDATA 1
#define BINTREERECURSE 1<<1

typedef struct
_S_bintree {
	BINTREEDATA* data;
	struct _S_bintree* left;
	struct _S_bintree* right;
} bintree;

bintree*
bintree_init
(
	void
)
{
	bintree* tree = malloc(sizeof(bintree));
	tree->data = BINTREENULL;
	tree->right = BINTREENULL;
	tree->left = BINTREENULL;
	return tree;
}

bool
bintree_hasleft
(
	bintree* tree
)
{
	return tree->left != BINTREENULL;
}

bool
bintree_hasright
(
	bintree* tree
)
{
	return tree->right != BINTREENULL;
}

void
bintree_delete
(
	bintree* tree,
	unsigned int flags
)
{
	if (tree == BINTREENULL) return;
	if (flags & BINTREERECURSE) {
		bintree_delete(tree->left, flags);
		bintree_delete(tree->right, flags);
	}
	if (flags & BINTREEFREEDATA) {
		free(tree->data);
	}
	free(tree);
}

void
bintree_test()
{
	bintree *treea = bintree_init(), *treeb = bintree_init(), *treec = bintree_init();
	int a = 1, b = 2, c = 3;
	treea->data = &a;
	treeb->data = &b;
	treec->data = &c;
	treea->left = treeb;
	treea->right = treec;
	
	printf("A: %d\n", *((int*)treea->data));
	if (bintree_hasleft(treea)) printf("B: %d\n", *((int*)treea->left->data));
	if (bintree_hasright(treea)) printf("C: %d\n", *((int*)treea->right->data));
	
	bintree_delete(treea, BINTREERECURSE | BINTREEFREEDATA);
	
	printf("%d %d", a, *((int*)treea->data));
}