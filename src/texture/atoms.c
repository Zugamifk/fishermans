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
texture_perlin( texture_gen_args args )
{
	unsigned int w = args.w;
	unsigned int h = args.h;
	
	BYTE *data = malloc(w*h*3);
	float r = rand()%1000;
	float g = rand()%1000;
	float b = rand()%1000;
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			int s = 0, d = 10;
			data[i] = 255*perlin_noise(x, y, w, h, r, s, d);
			data[i+1] = 255*perlin_noise(x, y, w, h, g, s, d);
			data[i+2] = 255*perlin_noise(x, y, w, h, b, s, d);
		}
	}
	return data;
}

BYTE*
texture_gradient( texture_gen_args args )
{
	unsigned int w = args.w;
	unsigned int h = args.h;
	
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
	texture_atom_id id
)
{
	const char* name;
	if (id >= TEXTURE_NUMATOMS) return NULL;
	else name = texture_atomnames[id];
	texture *t = texture_init(name, w, h);
	switch(id) {
		case TEXTURE_PERLIN: t->gen_cb = texture_perlin; break;
		case TEXTURE_GRADIENT: t->gen_cb = texture_gradient; break;
		default: break;
	}
	texture_generate(t);
	return t;
}