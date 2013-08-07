#define LANG_LINELEN 512
#define LANG_WORDLEN 128
#define LANG_SMALLCAP (1<<4)
#define LANG_MEDCAP (1<<8)
#define LANG_BIGCAP (1<<16)

#include "lang_scanner.c"
#include "lang_grammar.h"
#include "lang_tokenizer.c"
#include "lang_parser.c"