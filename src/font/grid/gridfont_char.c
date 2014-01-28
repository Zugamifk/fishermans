typedef struct 
gridfont_char_s
{
	bitvector *data;
	int height;
	int width;
} gridfont_char;

gridfont_char*
gridfont_char_init
(
	int *cells,
	int height,
	int width
)
{
	gridfont_char *gc = malloc(sizeof(gridfont_char));
	gc->pts = pts;
	gc->numpts = numpts;
	gc->w = w;
	gc->h = h;
	return gc;
}

void
gridfont_char_draw
(
	gridfont_char *c
)
{
	vec2* dot;
	glBegin(GL_QUADS);
	for (int i = 0; i < c->numpts; i++) {
		dot = c->pts[i];
		glVertex2d(dot->x,dot->y);
		glVertex2d(dot->x+1.0,dot->y);
		glVertex2d(dot->x+1.0,dot->y+1.0);
		glVertex2d(dot->x,dot->y+1.0);
	}
	glEnd();
}
