#define color_new3(r, g, b) color_new4(r, g, b, 1.0)

typedef struct
t_color
{
	double r;
	double g;
	double b;
	double a;
}	color;

typedef struct
t_colorpack
{
	color **colors;
	int size;
} colorpack;

color *
color_new4(double r, double g, double b, double a)
{
	color *c = malloc(sizeof(color));
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = a;
	return c;
}

colorpack *
color_newpack(color **c, int n)
{
	colorpack *cp = malloc(sizeof(colorpack));
	cp->colors = c;
	cp->size = n;
	return cp;
}

void
color_apply(color *c)
{
	glColor4d(c->r, c->g, c->b, c->a);
}

void
color_applyinverse(color *c)
{
	glColor4d(1.0-c->r, 1.0-c->g, 1.0-c->b, c->a);
}

void
color_applyhalftone(color* c) 
{
	glColor4d(0.5*c->r, 0.5*c->g, 0.5*c->b, c->a);
}

void
color_print(color* c) {
	printf("COLOR: [%f, %f, %f, %f]\n", c->r, c->g, c->b, c->a);
}