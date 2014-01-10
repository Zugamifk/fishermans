typedef enum
quadtree_nodetpye_e
{
	QUADTREE_NODE,
	QUADTREE_LEAF
} quadtree_nodetype;

typedef struct 
_quadtree_s
{
	void* data;
	struct _quadtree_s* subd;
	quadtree_nodetype type;
	unsigned int subdmask;
} quadtree;

quadtree*
quadtree_init
(
	unsigned int dim
)
{
	quadtree q = malloc(sizeof(kdtree));
	
	qd->data = NULL;
	int numbranches = 1<<dim;
	qd->subd = calloc(sizeof(void*), numbranches);
	qd->type = QUADTREE_LEAF;
	qd->subdmask = 1<<numbranches;
	
	return qd;
}

void
quadtree_insert
(
	
)
{

}