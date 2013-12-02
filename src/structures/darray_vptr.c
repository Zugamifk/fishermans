typedef uint32_t darray_uint_t;
#define DARRAY_WORDLEN 32

typedef enum 
_darray_vptr_resizewhen_e
{
	DARRAY_VPTR_MANUAL, // never resizes automatically
	DARRAY_VPTR_DOUBLING,	// treat like a stack
	DARRAY_VPTR_LOADSQUARED // ideal for hashtables
} _darray_vptr_resizewhen_e;
typedef struct
_darray_vptr_s
{
	_darray_vptr_resizewhen_e resizewhen;
	darray_uint_t cap;
	darray_uint_t load;
	void** d;
} darray_vptr_t;

darray_vptr_t*
darray_vptr_init
( darray_uint_t cap )
{
	darray_vptr_t* array = malloc(sizeof(darray_vptr_t));
	
	array->resizewhen = DARRAY_VPTR_DOUBLING;
	array->cap = BT_MAX(cap, 1);
	array->load = 0;
	array->d = calloc(sizeof(void*), array->cap);
	
	return array;
}

void
darray_vptr_resize
( darray_vptr_t* array, darray_uint_t size ) 
{
	void** d = array->d;
	darray_uint_t c = BT_MIN(array->cap, size);
	if (array->cap < DARRAY_MAXCAP && array->cap >= 1)
	{
		array->d = calloc(sizeof(void*), size);
		for (darray_uint_t i =0; i<c; i++) {
			array->d[i] = d[i];
		}
		array->cap = size;
	}
}

void
darray_vptr_add
( darray_vptr_t* array, void* val, darray_uint_t i )
{
	if (i >= array->cap || array->d[i] != NULL) return;
	(array->load)++;
	array->d[i] = val;
	
	switch(array->resizewhen) {
		case DARRAY_VPTR_MANUAL: break;
		case DARRAY_VPTR_DOUBLING: {
			if(array->load == array->cap) 
				darray_vptr_resize(array, (array->cap)<<1);
		} break;
		case DARRAY_VPTR_LOADSQUARED: {
			if(array->load * array->load >= array->cap) 
				darray_vptr_resize(array, (array->cap)<<1);
		} break;
	}
}

void
darray_vptr_remove
( darray_vptr_t* array, darray_uint_t i )
{
	if (i >= array->cap || array->d[i] == NULL) return;
	(array->load)--;
	array->d[i] = NULL;
	
	switch(array->resizewhen) {
		case DARRAY_VPTR_MANUAL: break;
		case DARRAY_VPTR_DOUBLING: {
			if(array->load == array->cap>>1) 
				darray_vptr_resize(array, (array->cap)>>1);
		} break;
		case DARRAY_VPTR_LOADSQUARED: {
			if(array->load * array->load < (array->cap)>>1) 
				darray_vptr_resize(array, (array->cap)>>1);
		} break;
	}
}

void
darray_vptr_free
( darray_vptr_t* array)
{
	free(array->d);
	free(array);
}

void
darray_vptr_test ()
{
	uint32_t i;
	for (darray_uint_t j = 1; j < DARRAY_MAXCAP+1; j<<=1) {
		printf("TESTING ON SIZE %d\n*********************\n", j);
		darray_vptr_t* a = darray_vptr_init(0);
		tick();
		for (i = 0; i < j; i++) {
			darray_vptr_add(a, (void*)(4), i);
		}
		i--;
		tock();
		
		tick();
		for (;i >0; i--) {
			free(a->d[i]);
			darray_vptr_remove(a, i);
		}
		tock();
		darray_vptr_free(a);
	}
}