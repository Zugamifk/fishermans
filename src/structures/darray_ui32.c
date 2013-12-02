#define DARRAY_MAXCAP (UINT_MAX>>8)
typedef struct
_darray_ui32_s
{
	unsigned int cap;
	unsigned int load;
	unsigned int* d;
} darray_ui32_t;

darray_ui32_t*
darray_ui32_init
( unsigned int cap )
{
	darray_ui32_t* array = malloc(sizeof(darray_ui32_t));
	
	array->cap = BT_MAX(cap, 1);
	array->load = 0;
	array->d = calloc(sizeof(unsigned int), array->cap);
	
	return array;
}

void
darray_ui32_resize
( darray_ui32_t* array ) 
{
	unsigned int* d = array->d;
	unsigned int c = array->cap;
	if (array->load == array->cap && array->cap < DARRAY_MAXCAP) // increase
	{
		array->d = malloc(sizeof(unsigned int) * (array->cap)<<1);
		for (unsigned int i =0; i<c; i++) {
			array->d[i] = d[i];
		}
		array->cap = (array->cap)<<1;
	} else
	if (array->load == array->cap/2 && array->cap > 1) // decrease
	{
		array->d = malloc(sizeof(unsigned int) * (array->cap)>>1);
		for (unsigned int i =0; i<c/2; i++) {
			array->d[i] = d[i];
		}
		array->cap = (array->cap)>>1;
	}
}

void
darray_ui32_add
( darray_ui32_t* array, unsigned int val, unsigned int i )
{
	if (i >= array->cap) return;
	(array->load)++;
	array->d[i] = val;
	darray_ui32_resize(array);
	
}

void
darray_ui32_remove
( darray_ui32_t* array, unsigned int i )
{
	if (i >= array->cap) return;
	(array->load)--;
	array->d[i] = 0;
	darray_ui32_resize(array);
}

void
darray_ui32_free
( darray_ui32_t* array )
{
	free(array->d);
	free(array);
}

void
darray_ui32_test ()
{
	darray_ui32_t* a = darray_ui32_init(0);
	unsigned int i;
	tick();
	for (i = 0; i < DARRAY_MAXCAP; i++) {
		darray_ui32_add(a, i, i);
	}
	i--;
	tock();
	tick();
	for (;i >0; i--) {
		darray_ui32_remove(a, i);
	}
	tock();
	darray_ui32_free(a);
}