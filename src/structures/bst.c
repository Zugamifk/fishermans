typedef bintree bstree;

bstree*
bst_init
(
	void
)
{
	return bintree_init();
}

void
bst_delete
(
	bstree* tree,
	unsigned int flags
)
{
	return bintree_delete(tree, flags | BINTREERECURSE);
}