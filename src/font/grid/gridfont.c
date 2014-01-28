#define GRIDFONT_NUMCHARS 256
#define GRIDFONT_MAXHEIGHT 256
#define GRIDFONT_MAXWIDTH 256
#define GRIDFONT_BASELINEDEFAULT(H) (0.2*(H))
#define GRIDFONT_MIDLINEDEFAULT(H) (0.6*(H))
#define GRIDFONT_LSPACEDEFAULT(W) (0.1*(W))
#define GRIDFONT_WSPACEDEFAULT(W) (W)

typedef enum
gridfont_state_e
{
	GRIDFONT_BAD,
	GRIDFONT_EMPTY,
	GRIDFONT_INITIALIZED,
	GRIDFONT_FULL
} gridfont_state;

typedef struct
gridfont_s
{
	gridfont_state state;
	int maxheight;
	int maxwidth;
	int baseline;
	int midline;
	int letterspace;
	int wordspace;
	bitvector *emptychar;
	bitvector **chars;
	int *widths;
} gridfont;

gridfont*
gridfont_init
(

)
{
	gridfont *gf = malloc(sizeof(gridfont));
	gf->state = GRIDFONT_EMPTY;
	return gf;
}

void
gridfont_parameterize
(
	gridfont *gf,
	int maxheight,
	int maxwidth,
	int baseline,
	int midline,
	int letterspace,
	int wordspace
)
{
	gf->maxheight = maxheight;
	gf->maxwidth = maxwidth;
	gf->baseline = baseline;
	gf->midline = midline;
	gf->letterspace = letterspace;
	gf->wordspace = wordspace;
	gf->emptychar = NULL;
	gf->chars = calloc(sizeof(bitvector *), GRIDFONT_NUMCHARS);
	gf->widths = calloc(sizeof(int), GRIDFONT_NUMCHARS);
	
	if (gf->state == GRIDFONT_EMPTY)gf->state = GRIDFONT_INITIALIZED;
}

bitvector *
gridfont_encodechar
(
	int *cells,
	int width,
	int height
)
{
	bitvector *gchar = bitvector_init(width * height);
	for (int i = 0; i < width * height; i++) {
		if (cells[i] == 1) 
			bitvector_set(gchar, i, 1);
	}
	return gchar;
}

void
gridfont_drawchar
(
	gridfont *gf,
	int c
)
{
	float x, y;
	if (gf->chars[c] == NULL) return;
	glPushMatrix();
	glBegin(GL_QUADS);
	x = 0.0, y = 0.0;
	for (int i = 0; i < gf->maxheight * gf->maxwidth; i++) {
		if (bitvector_get(gf->chars[c], i) == 1) {
			glVertex2d(x, y);
			glVertex2d(x+1.0, y);
			glVertex2d(x+1.0, y+1.0);
			glVertex2d(x, y+1.0);
		}
		if ((i+1) % gf->maxwidth == 0) {
			x -= (float)(gf->maxwidth-1);
			y += 1.0;
		} else {
			x += 1.0;
		}
	}
	glEnd();
	glPopMatrix();
}

void
gridfont_write
(
	gridfont* gf,
	char* str
)
{
	float x, y, nextpos;
	char *cptr, c;

	glPushMatrix();
	glBegin(GL_QUADS);
	x = 0.0, y = 0.0;
	for (cptr = str, c = *cptr; c != '\0'; cptr++, c = *cptr) {
		switch (c) {
			default: break;
		}
		if (gf->chars[(int)c] == NULL) {
			if (gf->emptychar != NULL) {
				for (int i = 0; i < gf->maxheight * gf->maxwidth; i++) {
					if (bitvector_get(gf->emptychar, i) == 1) {
						glVertex2d(x, y);
						glVertex2d(x+1.0, y);
						glVertex2d(x+1.0, y+1.0);
						glVertex2d(x, y+1.0);
					}
					if ((i+1) % gf->maxwidth == 0) {
						x -= (float)(gf->maxwidth-1);
						y += 1.0;
					} else {
						x += 1.0;
					}
				}	
			}
			nextpos = gf->wordspace;
		} else {
			for (int i = 0; i < gf->maxheight * gf->maxwidth; i++) {
				if (bitvector_get(gf->chars[(int)c], i) == 1) {
					glVertex2d(x, y);
					glVertex2d(x+1.0, y);
					glVertex2d(x+1.0, y+1.0);
					glVertex2d(x, y+1.0);
				}
				if ((i+1) % gf->maxwidth == 0) {
					x -= (float)(gf->maxwidth-1);
					y += 1.0;
				} else {
					x += 1.0;
				}
			}
			nextpos = gf->widths[(int)c];
		}
		y = 0.0;
		x += nextpos;
	}
	glEnd();
	glPopMatrix();
}
