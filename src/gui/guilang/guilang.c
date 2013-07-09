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