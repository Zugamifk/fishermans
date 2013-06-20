// A TOKEN/STRING PAIR
typedef struct
_S_guilang_tokenpair
{
	int token;
	char* string;
} _guilang_tokenpair;
	
_guilang_tokenpair*
guilang_makepair
(
	int token,
	char* string,
	_guilang_tokenpair* holder
)
{
	_guilang_tokenpair* tp;
	if (holder == NULL) {
		tp = malloc(sizeof(_guilang_tokenpair));
	} else tp = holder;
	tp->token = token;
	tp->string = malloc(GUILANG_LINELEN);
	strcpy(tp->string, string);
	
	return tp;
}	

void
guilang_deletepair
(
	_guilang_tokenpair* pair
)
{
	free(pair->string);
	free(pair);
}


int
_guilang_comparewords
(
	char* str,
	char** words,
	int num
)
{
	char noquotes[GUILANG_LINELEN];
	if (sscanf(str, "\'%[^\']\'", noquotes)==0) return 1;
	for (int i = 0; i < num; i++) {
		if (strcmp(noquotes, words[i]) == 0) return 0;
	}
	return 1;
}

void
guilang_init
(
)
{
}