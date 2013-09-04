#define FUNTABLEDATA void

typedef struct
_S_funtable
{
	FUNTABLEDATA** table;
	unsigned int len;
} funtable;

funtable*
funtable_init
(
	FUNTABLEDATA* (*fun)(int),
	unsigned int len
)
{
	funtable* ft = malloc(sizeof(funtable));
	ft->table = malloc(len);
	ft->len = len;
	
	for(int i = 0; i < len; i++) {
		ft->table[i] = fun(i);
	}
	
	return ft;
}

void
funtable_free
(
	funtable* ft,
	void (*freecb)(FUNTABLEDATA*)
)
{
	if (freecb == NULL) freecb = free;
	for (int i = 0; i < ft->len; i++) {
		freecb(ft->table[i]);
	}
	free(ft->table);
	free(ft);
}

FUNTABLEDATA* funtable_testcb(int i) 
{
	int* p = malloc(sizeof(int));
	*p = i;
	return p;
}

void
funtable_test
()
{
	funtable* ft = funtable_init(funtable_testcb, 100);
	for (int i = 0; i< ft->len; i++) {
		printf("%d ", *((int*)(ft->table[i])));
	}
	funtable_free(ft, NULL);
	
}