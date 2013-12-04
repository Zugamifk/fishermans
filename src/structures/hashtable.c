#define HASHTABLE
#define HASHTABLE_MAXSIZE	(1<<(sizeof(unsigned int)-1))
#define HASHTABLEDATA void
#define HASHTABLEPROBE 0

typedef void (*hashtable_printcb)(HASHTABLEDATA*);
typedef void (*hashtable_freecb)(HASHTABLEDATA*);

typedef struct
_S_hashtable_bucket
{
	char*	key;
	HASHTABLEDATA*	value;
}
_hashtable_bucket;

typedef struct
S_hashtable
{
	int load;
	int size;
	darray_vptr_t* data;
	int (*probecb)(int);
	float lfthreshold;
	unsigned int i;
	unsigned int hashseed;
}
hashtable;

int
_hashtable_hash
(
	hashtable* table,
	char*	key
)
{
	
	int hash = hashlittle(key, strlen(key), table->hashseed);
	// char* c = key;
	// while (*c != '\0') {
		// hash = ((hash<<5)^(hash>>27))^*c++;
	// }
	return hash;
}

_hashtable_bucket*
_hashtable_initbucket
(
	char*	key,
	HASHTABLEDATA*	value
)
{
	_hashtable_bucket* bucket = malloc(sizeof(_hashtable_bucket));
	
	bucket->key = malloc(strlen(key)+1);
	strcpy(bucket->key, key);
	bucket->value = value;

	return bucket;
}

int
hashtable_linearprobe
(
	int n
)
{
	return n+1;
}

int
hashtable_quadraticprobe
(
	int n
)
{
	return n*n;
}

hashtable*
hashtable_init
(
	int size
)
{
	hashtable* table = malloc(sizeof(hashtable));
	
	if (size == 0) {
		table->size = HASHTABLE_MAXSIZE;
	} else {
		table->size = size;
	}
	
	switch(HASHTABLEPROBE) {
		case 0: table->probecb = hashtable_linearprobe; table->lfthreshold = 1.0; break;
		case 1: table->probecb = hashtable_quadraticprobe; table->lfthreshold = 0.6; break;
	}
	
	table->data = darray_vptr_init(size);
	darray_vptr_setresize(table->data, DARRAY_VPTR_MANUAL);
	table->load = 0;
	table->i = 0;
	table->hashseed = 0xb00b135;
	
	return table;
}

int
hashtable_isfull
(
	hashtable* table
)
{
	return table->load == table->size;
}

int
hashtable_maxload
(
	hashtable* table
)
{
	return table->load >= (table->size)>>1;
}

void
hashtable_rehash
(
	hashtable* table,
	_hashtable_bucket** buckets,
	unsigned int num
)
{
	table->hashseed = table->hashseed + 0xC0FFEE;
	unsigned int hash, place, free, j;
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(table->data);
	for (int i = 0; i < num; i++) {
		_hashtable_bucket* b = buckets[i];
		hash = _hashtable_hash(table, b->key);
		place = hash%table->size;
		free = table->size - table->load;
		for (j = 0; j < free; j++) {
			if (data[place] == NULL) {
				darray_vptr_add(table->data, b, place);
				break;
			} else {
				place = (place+(table->probecb(i)))%table->size;
			}
		}
	}
}

void
hashtable_resize
(
	hashtable* table,
	unsigned int size
)
{
	if (table->load >= size || table->size > size) return;
	_hashtable_bucket* temparray[table->load];
	int ti = 0;
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(table->data);
	for (int i = 0; i < table->size; i++) {
		if (data[i] != NULL) {
			temparray[ti] = data[i];
			data[i] = NULL;
			ti++;
		}
	}
	darray_vptr_resize(table->data, size);
	table->size = size;
	hashtable_rehash(table, temparray, table->load);
}

void
hashtable_insert
(
	hashtable* table,
	char*	key,
	HASHTABLEDATA*	value
)
{
	printf("%s %d %d\n", key, table->size, table->load);

	if (hashtable_maxload(table)) hashtable_resize(table, (table->size)<<1); // TODO: REHASH/RESIZE
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(table->data);
	unsigned int hash = _hashtable_hash(table, key);
	unsigned int place = hash%table->size;
	int free = table->size - table->load;
	for (int i = 0; i < free; i++) {
		if (data[place] == NULL) {
			darray_vptr_add(table->data, _hashtable_initbucket(key, value), place);
			table->load = table->load + 1;
			return;
		} else {
			place = (place+(table->probecb(i)))%table->size;
		}
	}
		printf("!\n");

}

_hashtable_bucket*
_hashtable_getbucket
(
	hashtable* table,
	char* key,
	unsigned int* pos
)
{
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(table->data);
	unsigned int hash = _hashtable_hash(table, key);
	unsigned int place = hash%table->size;
	*pos = 0;
	_hashtable_bucket* bucket = NULL;
	for (int i = 0; i < table->size; i++) {
		bucket = data[place];
		if (bucket == NULL) {
			return NULL;
		} else
		if (strcmp(bucket->key, key) == 0) {
			*pos = place;
			return bucket;
		} else {
			place = (place+(table->probecb(i)))%table->size;
		}
	}
	return NULL;
}
	
HASHTABLEDATA*
hashtable_get
(
	hashtable*	table,
	char*	key
)
{
	unsigned int i;
	_hashtable_bucket* bucket = _hashtable_getbucket(table, key, &i);
	if (bucket == NULL) {
		return NULL;
	} else
	{
		return bucket->value;
	}

}

void
hashtable_remove
(
	hashtable*	table,
	char*		key
)
{
	unsigned int i;
	_hashtable_bucket* bucket = _hashtable_getbucket(table, key, &i);
	
	if (bucket == NULL) {
		// ERROR!!
	} else
	{
		free(bucket->key);
		free(bucket->value);
		free(bucket);
		darray_vptr_remove(table->data, i);
	}
}

void
hashtable_deepremove
(
	hashtable*	table,
	char*		key,
	hashtable_freecb cb
)
{
	unsigned int i;
	_hashtable_bucket* bucket = _hashtable_getbucket(table, key, &i);
	
	if (bucket == NULL) {
		// ERROR!!
	} else
	{
		free(bucket->key);
		if (cb != NULL) { 
			cb(bucket->value);
		}
		free(bucket);
		darray_vptr_remove(table->data, i);
	}
}

void
hashtable_begin
(
	hashtable* t,
	char** key,
	HASHTABLEDATA** d
)
{
	int i;
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(t->data);
	for (i = 0; ; i++) {
		if (i >= t->size) {
			*key = NULL;
			*d = NULL;
			t->i = i;
			return;
		}
		if (data[i] != NULL) break;
	}
	t->i = i;
	_hashtable_bucket* b = data[i];
	*key = b->key;
	*d = b->value;
}

bool
hashtable_end
(
	hashtable* t
)
{
	return t->i < t->size;
}

void
hashtable_next
(
	hashtable* t,
	char** key,
	HASHTABLEDATA** d
)
{	
	_hashtable_bucket** data = (_hashtable_bucket**)darray_vptr_getarray(t->data);
	int i = t->i + 1;
	while (i < t->size && data[i] == NULL) {
		i++;
	}
	t->i = i;
	if (t->i < t->size) {
		_hashtable_bucket* b = data[t->i];
		*key = b->key;
		*d = b->value;
		return;
	}
	*key = NULL;
	*d = NULL;
}
	
void
hashtable_deepfree
(
	hashtable* table,
	hashtable_freecb cb
)
{
	HASHTABLEDATA* v;
	char* k;
	for(hashtable_begin(table, &k, &v); 
		hashtable_end(table); 
		hashtable_next(table, &k, &v)) 
	{
		hashtable_deepremove(table, k, cb);
	}
	free(table->data);
	free(table);
}

void
hashtable_print
(
	hashtable*	table,
	hashtable_printcb datatostring
)
{
	printf("LOAD: %d\tCAPACITY: %d\tLOAD FACTOR THRESHOLD: %.2f\nDATA:{", table->load, table->size, table->lfthreshold);
	HASHTABLEDATA* d;
	char* k;
	for(hashtable_begin(table, &k, &d); hashtable_end(table); hashtable_next(table, &k, &d)) {
		printf("\t%s : ", k);
		datatostring(d);
		printf("\n");
	}
	printf("}\n");
}

void hashtable2string(HASHTABLEDATA* d) {printf("%d\n", d);}
void
hashtable_test()
{
	hashtable* table = hashtable_init(1);
	char* keys[10] = {"a","b","c","d","e","f","g","h","i","j"};
	for(int i =0; i<10; i++) {
		hashtable_insert(table, keys[i%10], (HASHTABLEDATA*)(&i));
		
	}
	hashtable_print(table, hashtable2string);
}