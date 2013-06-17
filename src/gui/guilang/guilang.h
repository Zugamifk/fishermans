#define GUILANG_LINELEN 512
#define GUILANG_DEBUG 1
#define GUILANG_PRINTTOKENSTRINGS 1

#define GUILANG_VERSION 1

#define GUILANG_USEDEFAULTSPEC 1
#ifdef GUILANG_USEDEFAULTSPEC
	#define GUILANG_DEFAULTSPEC ("guilang spec.txt")
#endif

#include "guilang.c"
#include "guilang_specification.c"
#include "guilang_tokenizer.c"