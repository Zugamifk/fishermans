typedef struct
_outlinechar_s
{
	bezier2d** segments;
	unsigned int numsegments;
} outlinechar;

outlinechar*
outlinechar_init
(
	vec2** pts,
	unsigned int* cts,
	unsigned int segct
)
{
	outlinechar *c = malloc(sizeof(outlinechar));
	c->segments = malloc(sizeof(bezier2d*)*segct);
	for(int i=0;i<segct;i++) {
		c->segments[i] = bezier2d_init(BEZIER2D_LINEAR, pts, cts[i], cts[i]);
		pts = pts + cts[i];
	}
	c->numsegments = segct;
	return c;
}