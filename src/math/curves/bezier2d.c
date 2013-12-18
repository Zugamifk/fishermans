typedef enum
_bezier2d_mode_e
{
	BEZIER2D_LINEAR,
	BEZIER2D_QUADRATIC,
	BEZIER2D_CUBIC
} _bezier2d_mode;

typedef struct
_bezier2d_s
{
	vec2** pts;
	unsigned int count;
	_bezier2d_mode mode;
} bezier2d;

// LINEAR BEZIER CURVES
// ===================================================
// B(t) = (1-t)P_0 + tP_1
void
bezier2d_generate_linear
(
	vec2** pts,
	unsigned int count,
	unsigned int destct,
	vec2*** dest
)
{
	*dest = malloc(sizeof(vec2*)*destct);
	double t = 0.0;
	double inc = (double)count/(double)destct;
	vec2 a, b, c;
	for(int i = 0, j=0; i < count-1; t += inc, j++) {
		if (t > 1.0) {
			t = t - 1.0;
			i++;
		}
		vec2_scale(&a, pts[i], 1.0-t);
		vec2_scale(&b, pts[i+1], t);
		vec2_add(&c, &a, &b);
		*dest[i] = vec2_copy(&c);
	}
}

bezier2d*
bezier2d_init
(
	_bezier2d_mode mode,
	vec2** pts,
	unsigned int count,
	unsigned int destct
)
{
	bezier2d* b = malloc(sizeof(bezier2d));

	if (destct < count) destct = count;
	b->count = destct;
	
	switch (mode) {
		case BEZIER2D_LINEAR: bezier2d_generate_linear(pts, count, destct, &(b->pts)); break;
		case BEZIER2D_QUADRATIC: break;
		case BEZIER2D_CUBIC: break;
	}
	
	return b;
}