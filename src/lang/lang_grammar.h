// ==========================================================
// LANG GRAMMAR DEFINITIONS
// // ==========================================================
#define LANG_LINELEN 512
#define LANG_WORDLEN 128
#define LANG_SMALLCAP (1<<4)
#define LANG_MEDCAP (1<<8)
#define LANG_BIGCAP (1<<16)
#define LANG_DEBUG 1
#define LANG_PRINTTOKENSTRINGS 0

//#define LANG_USEDEFAULTSPEC 1
#ifdef LANG_USEDEFAULTSPEC
	#define LANG_VERSION 1
	
	#if LANG_VERSION == 1
		#define LANG_DEFAULTSPEC ("langspec1")
	#endif
#endif

#ifdef LANG_DEBUG
//	#define LANG_SPEC_PRINTRESULT 1
//	#define LANG_PRINTPARSINGTABLE
//	#define LANG_PRINTFIRST
//	#define LANG_PRINTFOLLOW
//	#define LANG_PRINTNTFIRST
#endif //LANG_DEBUG
#define LANG_PARSER_MAXNESTINGDEPTH 256

#define LANG_STARTSYMBOL ("GUI")
#define LANG_ENDOFINPUTSTRING ("EOI")
#define LANG_GENERICWORD ("@")
#define LANG_GENERICNUM ("#")

const int _lang_tokencount = 7;
typedef enum
_E_lang_tokentype
{
	LANG_STRING,
	LANG_NUMBER,
	LANG_KEYWORD,
	LANG_NONTERMINAL,
	LANG_ENDOFSTRING,
	LANG_ENDOFINPUT,
	LANG_EPSILON
} _lang_tokentype;

const char* _lang_tokentypestrings[] ={
	"STRING",
	"NUMBER",
	"KEYWORD",
	"NONTERMINAL",
	"ENDOFSTRING",
	"ENDOFINPUT",
	"EPSILON"
};

const int _lang_keywordcount = 9;
const char* _lang_keywords[] = {
	"x", "y",
	"w", "h",
	"active", "inactive",
	"dimensions",
	"horizontal", "vertical"
};

// ==========================================================
//	LANG SPECIFICATION DEFINITIONS
// ==========================================================

#define LANGSPEC_PHRASELENMAX 128
#define LANGSPEC_WORDLENMAX 128
#define LANGSPEC_STACKSIZE 512
#define LANGSPEC_NUMTOKENTYPES 10
#define LANGSPEC_MAXTOKENSYMBOLSPERTOKENTYPE 16

// TOKEN TYPES
typedef enum
_E_langspec_tokentype
{
	LANGSPEC_TERMINAL,
	LANGSPEC_OPTPARENOPEN,
	LANGSPEC_OPTPARENCLOSE,
	LANGSPEC_SETPARENOPEN,
	LANGSPEC_SETPARENCLOSE,
	LANGSPEC_SETSEP,
	LANGSPEC_NONTERMINAL,
	LANGSPEC_ENDOFSTRING,
	LANGSPEC_TRANSITIONARROW,
	LANGSPEC_EPSILON
} _langspec_tokentype;

const char* _langspec_tokentypestrings[] = 
{
	"TERMINAL",
	"OPTPARENOPEN",
	"OPTPARENCLOSE",
	"SETPARENOPEN",
	"SETPARENCLOSE",
	"SETSEP",
	"NONTERMINAL",
	"ENDOFSTRING",
	"ARROW",
	"EPSILON"
};

const char* _langspec_tokensymbols[][LANGSPEC_MAXTOKENSYMBOLSPERTOKENTYPE] = {
	{ "@", "#", "$$" },
	{ "[", "$$"  },
	{ "]", "$$"  },
	{ "(", "$$"  },
	{ ")", "$$"  },
	{ "|", "$$"  },
	{ "$$" },
	{ "$$" },
	{ "->", "$$"  },
	{ "$$" }
};


// ==========================================================
// INCLUDES
// ==========================================================

#include "_lang_token.c"
#include "_lang_rule.c"

#include "_langspec_token.c"
#include "langspec_builder.c"
#include "lang_grammar.c"
