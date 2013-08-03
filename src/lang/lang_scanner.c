// ====================================================
// SCANNER
// ====================================================
typedef enum 
_E_lang_scanner_state {
	_LANG_SCANNER_STATE_ALPHA
,	_LANG_SCANNER_STATE_NUM
,	_LANG_SCANNER_STATE_OTHER
,	_LANG_SCANNER_STATE_SPACE
,	_LANG_SCANNER_STATE_NULL
} _lang_scanner_state;

_lang_scanner_state
_lang_scanner_getstate
(
	char c
)
{
	if (isalpha(c)) {
		return _LANG_SCANNER_STATE_ALPHA;
	} else 
	if (isdigit(c)) {
		return _LANG_SCANNER_STATE_NUM;
	} else
	if (isspace(c)) {
		return _LANG_SCANNER_STATE_SPACE;
	} else 
	if (c == '\0') {
		return _LANG_SCANNER_STATE_NULL;
	} else {
		return _LANG_SCANNER_STATE_OTHER;
	}
}

char**
lang_scan
(
	char* data
)
{
	char** lexemes = malloc(LANG_BIGCAP);
	
	char* curr = data;
	char* wordptr = data;
	_lang_scanner_state currstate, oldstate;
	currstate = oldstate = _lang_scanner_getstate(*curr);
	
	char** currword = lexemes;
	
	bool decimal = false;
	
	do {
		curr++;
		oldstate = currstate;
		currstate = _lang_scanner_getstate(*curr);
		
		if (currstate == oldstate) {
		
		} else
		if (oldstate == _LANG_SCANNER_STATE_SPACE)  {
			wordptr = curr;
		}else {
			
			// if it's a decimal point, continue
			if (*curr == '.' && !decimal) {
				decimal = true;
				continue;
			} else
			if (currstate == _LANG_SCANNER_STATE_NUM && decimal) {
				continue;
			}
		
			// make a new word
			unsigned int len = curr - wordptr;
			*currword = malloc(LANG_WORDLEN);
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