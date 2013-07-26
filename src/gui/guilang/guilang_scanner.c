// ====================================================
// SCANNER
// ====================================================
typedef enum 
_E_guilang_scanner_state {
	_GUILANG_SCANNER_STATE_ALPHA
,	_GUILANG_SCANNER_STATE_NUM
,	_GUILANG_SCANNER_STATE_OTHER
,	_GUILANG_SCANNER_STATE_SPACE
,	_GUILANG_SCANNER_STATE_NULL
} _guilang_scanner_state;

_guilang_scanner_state
_guilang_scanner_getstate
(
	char c
)
{
	if (isalpha(c)) {
		return _GUILANG_SCANNER_STATE_ALPHA;
	} else 
	if (isdigit(c)) {
		return _GUILANG_SCANNER_STATE_NUM;
	} else
	if (isspace(c)) {
		return _GUILANG_SCANNER_STATE_SPACE;
	} else 
	if (c == '\0') {
		return _GUILANG_SCANNER_STATE_NULL;
	} else {
		return _GUILANG_SCANNER_STATE_OTHER;
	}
}

char**
guilang_scan
(
	char* data
)
{
	char** lexemes = malloc(GUILANG_BIGCAP);
	
	char* curr = data;
	char* wordptr = data;
	_guilang_scanner_state currstate, oldstate;
	currstate = oldstate = _guilang_scanner_getstate(*curr);
	
	char** currword = lexemes;
	
	bool decimal = false;
	
	do {
		curr++;
		oldstate = currstate;
		currstate = _guilang_scanner_getstate(*curr);
		
		if (currstate == oldstate) {
		
		} else
		if (oldstate == _GUILANG_SCANNER_STATE_SPACE)  {
			wordptr = curr;
		}else {
			
			// if it's a decimal point, continue
			if (*curr == '.' && !decimal) {
				decimal = true;
				continue;
			} else
			if (currstate == _GUILANG_SCANNER_STATE_NUM && decimal) {
				continue;
			}
		
			// make a new word
			unsigned int len = curr - wordptr;
			*currword = malloc(GUILANG_WORDLEN);
			memcpy(*currword, wordptr, len);
			*((*currword)+len) = '\0';

			// advance pointers
			decimal = false;
			currword++;
			wordptr = curr;
		}
	} while (*curr != '\0');
	
	*currword = "$$";
	
	
	return lexemes;
}