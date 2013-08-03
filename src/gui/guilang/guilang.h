#define GUILANG_LINELEN 512
#define GUILANG_WORDLEN 128
#define GUILANG_SMALLCAP (1<<4)
#define GUILANG_MEDCAP (1<<8)
#define GUILANG_BIGCAP (1<<16)
#define GUILANG_DEBUG 1
#define GUILANG_PRINTTOKENSTRINGS 0

//#define GUILANG_USEDEFAULTSPEC 1
#ifdef GUILANG_USEDEFAULTSPEC
	#define GUILANG_VERSION 1
	
	#if GUILANG_VERSION == 1
		#define GUILANG_DEFAULTSPEC ("guilangspec1")
	#endif
#endif

#ifdef GUILANG_DEBUG
	#define GUILANG_SPEC_PRINTRESULT 1
//	#define GUILANG_PRINTPARSINGTABLE
//	#define GUILANG_PRINTFIRST
//	#define GUILANG_PRINTFOLLOW
//	#define GUILANG_PRINTNTFIRST
#endif //GUILANG_DEBUG
#define GUILANG_PARSER_MAXNESTINGDEPTH 256

#include "guilang.c"
#include "guilang_grammar.h"

#include "guilang_scanner.c"
#include "guilang_tokenizer.c"
#include "guilang_parser.c"

#include "guilang_compile.c"