typedef struct
_outlinecharset_s
{
	outlinechar* chars[FONTOUTLINE_SIZE];
	bool valid[FONTOUTLINE_SIZE];
} outlinecharset;

outlinecharset*
outlinecharset_load
(
	const char* filename
)
{
	char* data;
	int len;
	load(filename, &data, &len);
	
	for (char c=*data; c!='\0'; data++) {
		switch(c) {
			default: break;
		}
	}
}

void
outlinecharset_subdivide
(
	outlinecharset* chars,
	int depth
)
{
	
}