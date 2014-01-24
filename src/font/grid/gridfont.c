#define GRIDFONT_MAXHEIGHT 256
#define GRIDFONT_MAXWIDTH 256
#define GRIDFONT_BASELINEDEFAULT(H) (0.2*(H))
#define GRIDFONT_MIDLINEDEFAULT(H) (0.6*(H))
#define GRIDFONT_LSPACEDEFAULT(W) (0.1*(W))
#define GRIDFONT_WSPACEDEFAULT(W) (W)

typedef struct
gridfont_s
{
	int maxheight;
	int maxwidth;
	int baseline;
	int midline;
	int letterspace;
	int wordspace;
	gridfont_char *emptychar;
	gridfont_char **chars;
} gridfont;

static float state_fontx = 0.0;
static float state_fonty = 0.0;
static float state_scale = 1.0;

gridfont*
gridfont_init
(

)
{
	gridfont *gf = malloc(sizeof(gridfont));
	gf->chars = malloc(sizeof(gridfont_char*)*256);
	return gf;
}

void
gridfont_write
(
	gridfont* gf,
	char* str
)
{
	glPushMatrix();
	glTranslated(state_fontx, state_fonty, 0.0);
	glScaled(state_scale, state_scale, 1.0);
	for (char *c = str; *c != '\0'; c++) {
		gridfont_char_draw(gf->chars[(int)*c]);
	}
	glPopMatrix();
}

void
gridfont_pos
(
	double x,
	double y
)
{
	state_fontx = x;
	state_fonty = y;
}

void
gridfont_scale
(
	double s
)
{
	state_scale = s;
}