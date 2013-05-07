#define	FONT_LEN_INF	1000.0

typedef struct
fontchar
{
	vec2 **pts;
	int ptct;
	vec2 ***edges;
	int edgect;
	double width;
} fontchar;

typedef struct
fontcharset
{
	fontchar **set;
	double spacing;
	int complete;
} fontcharset;

typedef struct
fontinfo
{
	fontcharset *font;
	color *col;
} fontinfo;
fontinfo GlobalFontInfo;

fontinfo *		font_init(const char *fname, color *c);
fontinfo *		font_initdef();
void			font_draw(char *str, fontinfo *fi, double x, double y, double s, double w);
double			font_getstrwidth(char *str, fontinfo *fi);

#define CHARSET_MAXPTS 		16
#define CHARSET_MAXEDGES	16

#include "fchar.c"
#include "charset.c"
#include "font.c"
