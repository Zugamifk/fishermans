#define GUILANG_LINELEN 512
#define GUILANG_DEBUG 1
#define GUILANG_PRINTTOKENSTRINGS 0

#define GUILANG_VERSION 1

#define GUILANG_USEDEFAULTSPEC 1
#ifdef GUILANG_USEDEFAULTSPEC
	#define GUILANG_DEFAULTSPEC ("guilang spec.txt")
#endif

const int _guilang_tokencount = 12;
typedef enum
_E_guilang_token
{
	GUILANG_GUIHEAD,
	GUILANG_WINDOWHEAD,
	GUILANG_CELLHEAD,
	GUILANG_KEYWORD,
	GUILANG_ARGPARENOPEN,
	GUILANG_ARGPARENCLOSE,
	GUILANG_BODYPARENOPEN,
	GUILANG_BODYPARENCLOSE,
	GUILANG_NUMBER,
	GUILANG_STRING,
	GUILANG_SPECIALCHAR,
	GUILANG_ENDOFSTRING
	
} _guilang_token;

const char* _guilang_tokenstrings[] ={
	"GUIHEAD",
	"WINDOWHEAD",
	"CELLHEAD",
	"KEYWORD",
	"ARGPARENOPEN",
	"ARGPARENCLOSE",
	"BODYPARENOPEN",
	"BODYPARENCLOSE",
	"NUMBER",
	"STRING",
	"SPECIALCHAR",
	"ENDOFSTRING"
};

const char* _guilang_tokenwords[] = {
	"GUI",
	"WINDOW",
	"CELL",
	"",
	"(",
	")",
	"{",
	"}",
	"",
	"",
	"",
	"ENDOFSTRING"
};

const int _guilang_keywordcount = 8;
const char* _guilang_keywords[] = {
	"x", "y"
,	"w", "h"
,	"horizontal", "vertical"
,	"active", "inactive"
};

const char _guilang_specialcharcount = 2;
const char _guilang_specialchars[] = {
	':', ','
};

#include "guilang.c"
#include "guilang_specification.c"
#include "guilang_tokenizer.c"

#include "guilang_compile.c"