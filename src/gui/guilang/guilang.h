// Guilang variable names
#define GUIVAR_GUIWIDTH ("WIDTH")
#define GUIVAR_GUIHEIGHT ("HEIGHT")

const char* GUILANG_VERSIONS[] = {
	"guilangspec1"
};

typedef enum
_guilang_version
{
	GUILANGSPEC1
} guilang_version;

#define GUILANG_LATEST GUILANGSPEC1

#include "guilang_processor.c"
#include "guilang.c"
#include "guilang_compile.c"