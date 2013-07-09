// ==========================================================
// GUILANG GRAMMAR DEFINITIONS
// // ==========================================================

const int _guilang_tokencount = 7;
typedef enum
_E_guilang_tokentype
{
	GUILANG_STRING,
	GUILANG_NUMBER,
	GUILANG_KEYWORD,
	GUILANG_NONTERMINAL,
	GUILANG_ENDOFSTRING,
	GUILANG_ENDOFINPUT,
	GUILANG_EPSILON
} _guilang_tokentype;

const char* _guilang_tokentypestrings[] ={
	"STRING",
	"NUMBER",
	"KEYWORD",
	"NONTERMINAL",
	"ENDOFSTRING",
	"ENDOFINPUT",
	"EPSILON"
};

// ==========================================================
//	GUILANG SPECIFICATION DEFINITIONS
// ==========================================================

#define GUILANGSPEC_PHRASELENMAX 128
#define GUILANGSPEC_WORDLENMAX 128
#define GUILANGSPEC_STACKSIZE 512
#define GUILANGSPEC_NUMTOKENTYPES 10
#define GUILANGSPEC_MAXTOKENSYMBOLSPERTOKENTYPE 16

// TOKEN TYPES
typedef enum
_E_guilangspec_tokentype
{
	GUILANGSPEC_TERMINAL,
	GUILANGSPEC_OPTPARENOPEN,
	GUILANGSPEC_OPTPARENCLOSE,
	GUILANGSPEC_SETPARENOPEN,
	GUILANGSPEC_SETPARENCLOSE,
	GUILANGSPEC_SETSEP,
	GUILANGSPEC_NONTERMINAL,
	GUILANGSPEC_ENDOFSTRING,
	GUILANGSPEC_TRANSITIONARROW,
	GUILANGSPEC_EPSILON
} _guilangspec_tokentype;

const char* _guilangspec_tokentypestrings[] = 
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

const char* _guilangspec_tokensymbols[][GUILANGSPEC_MAXTOKENSYMBOLSPERTOKENTYPE] = {
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

#include "_guilang_token.c"
#include "_guilang_rule.c"

#include "_guilangspec_token.c"
#include "guilangspec_builder.c"
#include "guilang_grammar.c"