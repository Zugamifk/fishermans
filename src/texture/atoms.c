typedef enum
_texture_atom_id_e
{
	TEXTURE_PERLIN,
	TEXTURE_GRADIENT,
	TEXTURE_NUMATOMS
} texture_atom_id;

const char* texture_atomnames[2] =
{
	"PERLIN",
	"GRADIENT"
};

BYTE*
texture_perlin( texture* t )
{
	unsigned int w = t->width;
	unsigned int h = t->height;
	
	BYTE *data = malloc(w*h*3);
	float *r = hashtable_get(t->vars, "RED");
	float *g = hashtable_get(t->vars, "BLUE");
	float *b = hashtable_get(t->vars, "GREEN");
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			int s = 0, d = 10;
			data[i] = 255*perlin_noise(x, y, w, h, *r+TIME, s, d);
			data[i+1] = 255*perlin_noise(x, y, w, h, *g+TIME, s, d);
			data[i+2] = 255*perlin_noise(x, y, w, h, *b+TIME, s, d);
		}
	}
	return data;
}

static unsigned int prec_exp = 8;
BYTE*
texture_perlin2( texture* t )
{
	unsigned int w = t->width;
	unsigned int h = t->height;
	
	BYTE *data = malloc(w*h*3);
	float *r = hashtable_get(t->vars, "RED");
	float *g = hashtable_get(t->vars, "BLUE");
	float *b = hashtable_get(t->vars, "GREEN");
	*r = rand()%1000;
	*g = rand()%1000;
	*b = rand()%1000;
	
	unsigned int *sd = hashtable_get(t->vars, "START");
	unsigned int *d = hashtable_get(t->vars, "DEPTH");
	
	unsigned int *rc, *gc, *bc;
	noise_data perlin_info;
	int pdd[2] = {w, h};
	perlin_info.dims = pdd;
	perlin_info.ndims = 2;
	perlin_info.max = 256<<prec_exp;
	perlin_info.type = NOISE_PERLIN;
	if (d == NULL) {
		perlin_info.perlin_depth = 8;
	} else {
		perlin_info.perlin_depth = *d;
	}
	if (sd == NULL) {
		perlin_info.perlin_startdepth = 0;
	} else {
		perlin_info.perlin_startdepth = *sd;
	}
	
	perlin_info.seed = (unsigned int)(*r);
	rc = perlin_generate2dui(&perlin_info);
	
	perlin_info.seed = (unsigned int)(*g);
	gc = perlin_generate2dui(&perlin_info);
	
	perlin_info.seed = (unsigned int)(*b);
	bc = perlin_generate2dui(&perlin_info);
	
	int i;
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			i = (x*h + y);
			data[i*3] = (BYTE)(rc[i]>>prec_exp);
			data[i*3+1] = (BYTE)(gc[i]>>prec_exp);
			data[i*3+2] = (BYTE)(bc[i]>>prec_exp);
		}
	}
	free(rc);
	free(gc);
	free(bc);
	return data;
}

BYTE*
texture_gradient( texture* t )
{
	unsigned int w = t->width;
	unsigned int h = t->height;
	
	BYTE *data = malloc(w*h*3);
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			double px = (double)x/(double)w;
			double py = (double)y/(double)h;
			data[i] = 0;
			data[i+1] = 255*px;
			data[i+2] = 255*py;
		}
	}
	return data;
}

texture*
texture_initatom
(
	unsigned int w,
	unsigned int h,
	texture_atom_id id,
	hashtable* vars
)
{
	const char* name;
	if (id >= TEXTURE_NUMATOMS) return NULL;
	else name = texture_atomnames[id];
	texture *t = texture_init(name, w, h, vars);
	switch(id) {
		case TEXTURE_PERLIN: {
			t->gen_cb = texture_perlin2; 
			
			float *rgb = malloc(sizeof(float)*3);
			rgb[0] = rand()%1000;
			rgb[1] = rand()%1000;
			rgb[2] = rand()%1000;
			hashtable_insert(t->vars, "RED", rgb);
			hashtable_insert(t->vars, "GREEN", rgb+1);
			hashtable_insert(t->vars, "BLUE", rgb+2);
			
			hashtable_insert(t->vars, "ATOM", (HASHTABLEDATA*)texture_atomnames[id]);
			}break;
		case TEXTURE_GRADIENT: t->gen_cb = texture_gradient; break;
		default: break;
	}
	texture_generate(t);
	return t;
}