typedef uint32_t bitvector_block_t;
#define BITVECTOR_BLOCKSIZE 32
typedef struct
bitvector_t
{
	bitvector_block_t *bits;
	int len;
	int blocknum;
} bitvector;

bitvector*
bitvector_init
(
	int len
)
{
	bitvector *bv = malloc(sizeof(bitvector));
	bv->blocknum = (len-1)/BITVECTOR_BLOCKSIZE + 1;
	bv->len = len;
	bv->bits = calloc(sizeof(bitvector_block_t), bv->blocknum);
	return bv;
}

bool
bitvector_get
(
	bitvector *b,
	int i
)
{
	if (i < 0 || i >= b->len) {
		return false;
	}
	
	int blocki = i/BITVECTOR_BLOCKSIZE;
	int blocksubi = i%BITVECTOR_BLOCKSIZE;
	return BT_BIT(b->bits[blocki], blocksubi);
}

void
bitvector_set
(
	bitvector *b,
	int i,
	bool v
)
{
	if (i < 0 || i >= b->len) {
		return;
	}
	
	int blocki = i/BITVECTOR_BLOCKSIZE;
	int blocksubi = i%BITVECTOR_BLOCKSIZE;
	if (v) b->bits[blocki] = b->bits[blocki] | v<<blocksubi;
	else b->bits[blocki] = b->bits[blocki] & ~(v<<blocksubi);
}

void
bitvector_delete
(
	bitvector *b
)
{
	free(b->bits);
	free(b);
}

void
bitvector_print
(
	bitvector *b
)
{
	printf("BITVECTOR: ");
	for(int i = 0; i < b->blocknum; i++) {
		for (int j = 0; j < BITVECTOR_BLOCKSIZE; j++) {
			printf("%d", BT_BIT(b->bits[i], j));
		}
		printf(" ");
	}
	printf("\tLEN: %d", b->len);
	printf("\tNUMBLOCKS: %d\n", b->blocknum);
}

void
bitvector_test
(void)
{
	bitvector *b = bitvector_init(128);
	bitvector_print(b);
	int k = 0;
	for (int i = 0, j = 1; j < 128;) {
		bitvector_set(b, j, 1);
		bitvector_print(b);
		k = j;
		j = i+k;
		i = k;
	}
	for (int i = 0, j = 1; j < 128;) {
		int v = bitvector_get(b, j);
		printf("INDEX %d = %d", j, v);
		k = j;
		j = i+k;
		i = k;
	}
	bitvector_delete(b);
}