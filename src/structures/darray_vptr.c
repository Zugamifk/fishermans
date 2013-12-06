#define DARRAY_WORDLEN 32
typedef uint32_t darray_uint_t;
#define DARRAY_NULLINDEX 0
#define DARRAY_MAXCAP (UINT_MAX>>8)
#define DARRAY_MINCAP 1

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
	darray_uint_t lastindex;
	void** d;
} darray_vptr_t;

darray_vptr_t*
darray_vptr_init
( darray_uint_t cap )
{
	darray_vptr_t* array = malloc(sizeof(darray_vptr_t));
	
	array->resizewhen = DARRAY_VPTR_DOUBLING;
	array->cap = BT_MAX(cap, DARRAY_MINCAP);
	array->lastindex = DARRAY_NULLINDEX;
	array->d = calloc(sizeof(void*), array->cap);
	return array;
}

void
darray_vptr_setresize
( darray_vptr_t* array, _darray_vptr_resizewhen_e event )
{
	array->resizewhen = event;
}

void*
darray_vptr_get
( darray_vptr_t* array, darray_uint_t i )
{
	return array->d[i];
}

void**
darray_vptr_getarray
( darray_vptr_t* array)
{
	return array->d;
}

void
darray_vptr_resize
( darray_vptr_t* array, darray_uint_t size ) 
{
	void** d = array->d;
	darray_uint_t c = BT_MIN(array->cap, size);
	if (array->cap < DARRAY_MAXCAP && array->cap >= DARRAY_MINCAP)
	{
		array->d = calloc(sizeof(void*), size);
		for (darray_uint_t i =0; i<c; i++) {
			array->d[i] = d[i];
		}
		array->cap = size;
		//free(d);
	}
}

void
darray_vptr_add
( darray_vptr_t* array, void* val, darray_uint_t i )
{
	darray_uint_t cap = array->cap;
	if (i >= cap) {
		switch(array->resizewhen) {
			case DARRAY_VPTR_MANUAL: return;
			case DARRAY_VPTR_DOUBLING: {
				darray_vptr_resize(array, cap<<1);
			} break;
			case DARRAY_VPTR_LOADSQUARED: {
				darray_vptr_resize(array, cap*cap);
			} break;
		}
		darray_vptr_add(array, val, i);
	}
	array->d[i] = val;
	array->lastindex = BT_MAX(i, array->lastindex);
}

void
darray_vptr_remove
( darray_vptr_t* array, darray_uint_t i )
{
	darray_uint_t cap = array->cap;
	if (i >= cap || array->d[i] == NULL) return;
	array->d[i] = NULL;
	if (i == array->lastindex)
		switch(array->resizewhen) {
			case DARRAY_VPTR_MANUAL: break;
			case DARRAY_VPTR_DOUBLING: {
				darray_uint_t j;
				for(j = i-1; 
					j != DARRAY_NULLINDEX && array->d[j] == NULL; 
					j--) {}
				array->lastindex = j;
				while (array->cap > DARRAY_MINCAP && j < (array->cap)>>1)
					darray_vptr_resize(array, (array->cap)>>1);
			} break;
			case DARRAY_VPTR_LOADSQUARED: {
				darray_uint_t j;
				for(j = i-1; 
					j != DARRAY_NULLINDEX && array->d[j] == NULL; 
					j--) {}
				array->lastindex = j;
				darray_uint_t nc = (darray_uint_t)sqrt((double)cap);
				while (array->cap > DARRAY_MINCAP && j < nc)
					darray_vptr_resize(array, nc);
					nc = (darray_uint_t)sqrt((double)(array->cap));
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
darray_vptr_print
( darray_vptr_t* array )
{
	printf("DYNAMIC POINTER ARRAY (VOID*)\n***************************\n");
	char* resizestr[3] = {"MANUAL", "DOUBLING", "SQUARING"};
	printf("RESIZE WHEN: %s\n", resizestr[array->resizewhen]);
	printf("CAPACITY: %d\n", array->cap);
	printf("LAST INDEX: %d\n", array->lastindex);
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
		darray_vptr_print(a);
		
		tick();
		for (;i >0; i--) {
			free(a->d[i]);
			darray_vptr_remove(a, i);
		}
		tock();
		darray_vptr_print(a);
		darray_vptr_free(a);
	}
}