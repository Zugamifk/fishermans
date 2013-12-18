typedef struct
_outlinefont_s
{
	outlinecharset* set;
	void (*drawcb)(struct _outlinefont_s*, char*);
} outlinefont;
typedef void (*outlinefont_drawcb)(outlinefont*, char*);

outlinefont*
outlinefont_init
(
	const char* filename,
	int curvesubmax
)
{
	outlinefont* font = malloc(sizeof(outlinefont));
	font->set = outlinecharset_load(filename);
	outlinecharset_subdivide(font->set, curvesubmax);
	font->drawcb = NULL;
	return font;
}