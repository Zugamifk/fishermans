typedef enum
debug_outputmask_e
{
	DEBUG_FMT_NONE = 0,
	DEBUG_FMT_NAME,
	DEBUG_FMT_TAG,
	DEBUG_FMT_NAMETAG,
	DEBUG_FMT_INDEX,
	DEBUG_FMT_INDEXVAL_PTR,
	DEBUG_FMT_NAMEINDEX,
	DEBUG_FMT_NAMEVAL_PTR
} debug_outputmask_t;

#define DEBUG_TYPE void*
typedef void (*debug_typestring_cb)(DEBUG_TYPE);

void
debug_ptr( void* ptr, debug_outputmask_t mask, ... )
{
	va_list val;
	va_start(val, mask);
	switch (mask) {
		case DEBUG_FMT_NONE: 
			printf("ptr: %#x\n", ptr); 
			break;
		case DEBUG_FMT_NAME: {
			char* name = va_arg(val, char*);
			printf("%s: %#x\n", name, ptr);
		} break;
		case DEBUG_FMT_TAG: {
			char* tag = va_arg(val, char*);
			printf("ptr: %#x --> %s\n", ptr, tag);
		} break;
		case DEBUG_FMT_NAMETAG: {
			char* name = va_arg(val, char*);
			char* tag = va_arg(val, char*);
			printf("%s: %#x --> %s\n", name, ptr, tag);
		} break;
		case DEBUG_FMT_INDEX: {
			unsigned int index = va_arg(val, unsigned int);
			printf("ptr[%u]: %#x\n", index, ptr);
		} break;
		case DEBUG_FMT_INDEXVAL_PTR: {
			unsigned int index = va_arg(val, unsigned int);
			unsigned int value = va_arg(val, unsigned int);
			printf("ptr[%u]: %#010x = %#010x\n", index, ptr, value);
		} break;
		case DEBUG_FMT_NAMEINDEX: {
			char* name = va_arg(val, char*);
			unsigned int index = va_arg(val, unsigned int);
			printf("%s[%u]: %#x\n", name, index, ptr);
		} break;
		case DEBUG_FMT_NAMEVAL_PTR: {
			char* name = va_arg(val, char*);
			unsigned int value = va_arg(val, unsigned int);
			printf("%s: %#x = %#x\n", name, ptr, value);
		}
	}
	va_end(val);
}

void
debug_array(void* array, unsigned int len) {
	printf("PRINTING ARRAY:\n**********************\n");
	for (unsigned int i = 0; i < len; i++) {
		debug_ptr(array+i, DEBUG_FMT_INDEXVAL_PTR, i, ((unsigned int*)array)[i]);
	}
	printf("**********************\n");
}

void debug_intstr(DEBUG_TYPE d)
{
	printf("INT: %d\n", d);
}

void
debug_arrayt(void* array, unsigned int len, debug_typestring_cb str_cb) {
	unsigned int* ptrvals = (unsigned int*)array;
	printf("PRINTING ARRAY:\n**********************\n");
	for (unsigned int i = 0; i < len; i++) {
		debug_ptr(array+i, DEBUG_FMT_INDEXVAL_PTR, i, ptrvals[i]);
		if (ptrvals[i] != 0) {
			printf("\t");
			str_cb((DEBUG_TYPE)ptrvals[i]);
		}
	}
	printf("**********************\n");
}