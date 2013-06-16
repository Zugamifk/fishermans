#define HASHTABLE_MAXSIZE	256
#define HASHTABLEDATA void
#define HASHTABLEPROBE 0

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
	_hashtable_bucket**	data;
	int (*probecb)(int);
	float lfthreshold;
}
hashtable;

int
_hashtable_hash
(
	char*	key
)
{
	
	int hash = hashlittle(key, strlen(key), 0xb00b135);
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
	
	table->data = calloc(table->size, sizeof(_hashtable_bucket*));
	table->load = 0;
	
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

void
hashtable_insert
(
	hashtable* table,
	char*	key,
	HASHTABLEDATA*	value
)
{

	if (hashtable_isfull(table)) return;
	
	_hashtable_bucket** data = table->data;
	unsigned int hash = _hashtable_hash(key);
	unsigned int place = hash%table->size;
	int free = table->size - table->load;

	for (int i = 0; i < free; i++) {
		if (data[place] == NULL) {
			data[place] = _hashtable_initbucket(key, value);
			table->load = table->load + 1;
			return;
		} else {
			place = (place+table->probecb(i))%table->size;
		}
	}
}

_hashtable_bucket*
_hashtable_getbucket
(
	hashtable* table,
	char* key
)
{
	_hashtable_bucket** data = table->data;
	int hash = _hashtable_hash(key);
	int place = hash%table->size;
	_hashtable_bucket* bucket = NULL;
	float loadfactor = (float)table->load/(float)table->size;
	for (int i = 0; i < table->size; i++) {
		bucket = data[place];
		if (bucket == NULL || (loadfactor > table->lfthreshold && _hashtable_hash(bucket->key) ^ hash)) {
			return NULL;
		} else if (strcmp(bucket->key, key) == 0) {
			return bucket;
		} else {
			place = (place+table->probecb(i))%table->size;
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

	_hashtable_bucket* bucket = _hashtable_getbucket(table, key);
	
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

	_hashtable_bucket* bucket = _hashtable_getbucket(table, key);
	
	if (bucket == NULL) {
		// ERROR!!
	} else
	{
		free(bucket->key);
		free(bucket->value);
		free(bucket);
	}
}

void
hashtable_print
(
	hashtable*	table,
	void (*datatostring)(void*)
)
{
	printf("LOAD: %d\tCAPACITY: %d\tLOAD FACTOR THRESHOLD: %.2f\nDATA:{", table->load, table->size, table->lfthreshold);
	for (int i = 0; i < table->size; i++) {
		if (table->data[i] != NULL) {
			printf("\t%s : ", table->data[i]->key);
			datatostring(table->data[i]->value);
			printf("\n");
		}
	}
	printf("}\n");
}