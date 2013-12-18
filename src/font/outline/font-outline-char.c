typedef struct
_outlinechar_s
{
	bezier2d* outline;
} outlinechar;

outlinechar*
outlinechar_init
(
	vec2** pts,
	unsigned int num
)
{
	outlinechar *c = malloc(sizeof(outlinechar));
	c->outline = bezier2d_init(BEZIER2D_LINEAR, pts, num, num);
	return c;
}