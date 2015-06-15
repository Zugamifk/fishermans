// ==================================================
// GRIDFONT FILE OPS
// ==================================================
// STRUCTURE OF A FILE:
// WDITH HEIGHT 
// BASELINE MIDLINE
// LETTERSPACE WORDSPACE
// EMPTY CHAR
// CHARS

// STRUCTURE OF A CHAR
// CHAR WIDTH BITVECTOR
#define GRIDFONT_ALLOC (1<<14)

void
gridfont_save
(
	gridfont *gf,
	char *filename
)
{
	char *data = calloc(GRIDFONT_ALLOC, 1);
	char *cursor = data;
	int line;
	
	line = sprintf(cursor, "%d %d\n", gf->maxwidth, gf->maxheight);
	cursor += line;
	
	line = sprintf(cursor, "%d %d\n", gf->baseline, gf->midline);
	cursor += line;
	
	line = sprintf(cursor, "%d %d\n", gf->letterspace, gf->wordspace);
	cursor += line;
	
	//line = bitvector_tochars(gf->emptychar, cursor);
	//cursor += line;
	//*cursor = '\n';
	//cursor++;
	
	for (int i = 0; i < GRIDFONT_NUMCHARS; i++) {
		if (gf->chars[i] == NULL) continue;
		line = sprintf(cursor, "%c %d ", (char)i, gf->widths[i]);
		cursor += line;
		
		line = bitvector_tochars(gf->chars[i], cursor);
		cursor += line;
		*cursor = '\n';
		cursor++;
	}
	save(filename, data, cursor-data);
}