// ==========================================================
// LANG GRAMMAR DEFINITIONS
// // ==========================================================
#define LANG_DEBUG 1
#define LANG_PRINTTOKENSTRINGS 0

#ifdef LANG_DEBUG
//	#define LANG_SPEC_PRINTRESULT 1
//	#define LANG_PRINTPARSINGTABLE
//	#define LANG_PRINTFIRST
//	#define LANG_PRINTFOLLOW
//	#define LANG_PRINTNTFIRST
#endif //LANG_DEBUG
#define LANG_PARSER_MAXNESTINGDEPTH 256

#define LANG_ENDOFINPUTSTRING ("EOI")
#define LANG_GENERICWORD ("@")
#define LANG_GENERICNUM ("#")

const int _lang_tokencount = 7;
typedef enum
_E_lang_tokentype
{
	LANG_USERSTRING,
	LANG_STRING,
	LANG_NUMBER,
	LANG_NONTERMINAL,
	LANG_ENDOFSTRING,
	LANG_ENDOFINPUT,
	LANG_EPSILON
} _lang_tokentype;

const char* _lang_tokentypestrings[] ={
	"USERSTRING",
	"STRING",
	"NUMBER",
	"NONTERMINAL",
	"ENDOFSTRING",
	"ENDOFINPUT",
	"EPSILON"
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
