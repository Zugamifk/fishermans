typedef enum
kdtree_nodetpye_e
{
	KDTREE_NODE,
	KDTREE_LEAF
} kdtree_nodetype;

typedef struct 
_kdtree_s
{
	void* data;
	struct _kdtree_s* subd;
	kdtree_nodetype type;
	unsigned int subdmask;
} kdtree;

kdtree*
kdtree_init
(
	unsigned int dim
)
{
	kdtree kd = malloc(sizeof(kdtree));
	
	kd->data = NULL;
	int numbranches = 1<<dim;
	kd->subd = calloc(sizeof(void*), numbranches);
	kd->type = KDTREE_LEAF;
	kd->subdmask = 1<<numbranches;
	
	return kd;
}