#define DARRAY_MAXCAP (1<<(sizeof(unsigned int)))
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
	darray_ui32_t array = malloc(sizeof(darray_ui32_t));
	
	array->cap = BT_MAX(cap, 1);
	array->load = 0;
	array->d = calloc(sizeof(unsigned int), array->cap);
	
	return array;
}