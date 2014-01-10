static unsigned int prng(unsigned int word, unsigned int seed)
{
	word*=8;
	unsigned int bit = 0;
	unsigned int j = 0;
	unsigned int lfsr = seed;
	for (int c = 0; c < word; c++) {
		j = 0;
		bit = 0;
		for (int i = word; j < word; i >>= 1) {
			bit = bit ^ BT_BIT(lfsr, j);
			j += word - i + 1;
		}
		lfsr = (lfsr >> 1) | (bit << (word-1) );
	}
	return lfsr;
}

static unsigned int lerp(unsigned int a, unsigned int b, float t) {
	return (unsigned int)((t*(float)a)+((1.0-t)*(float)b));
}

static unsigned int cerp(unsigned int a, unsigned int b, float t) {
	t = 0.5 - 0.5*cos(t*3.14159);
	return (unsigned int)((t*(float)a)+((1.0-t)*(float)b));
}

unsigned int*
perlin_generate2dui
(
	noise_data* data
)
{
	unsigned int word = sizeof(unsigned int); // length of a word
	unsigned int pregenside = (1<<data->perlin_depth)+1;
	unsigned int *pregen = malloc(sizeof(unsigned int)*pregenside*pregenside*2);
	unsigned int cursor = 0;
 
	for (int l = data->perlin_startdepth; l < data->perlin_depth; l++) {
		int side = (1 << l) + 1;
		for (int i = 0; i < side*side; i++) {
			pregen[cursor] = prng(word, cursor + data->seed); // pseudo-rng
			cursor++;
		}
	}
	unsigned int size = data->dims[0] * data->dims[1];
	unsigned int *noise = malloc(sizeof(unsigned int)*size);
	
	unsigned int val;
	unsigned int d;
	unsigned int	w = data->dims[0], 
					h = data->dims[1], 
					x = 0, 
					y = 0,
					tl, tr, bl, br, lx, ly, step, prw, side;
	unsigned int span;
	unsigned int lng = BT_MAX(w, h);
	for (span = 1; span < lng; span <<= 1) {}
	float tx, ty;
	unsigned int pp;
	
	unsigned int biggest = 1000;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			val = 0;
			d = data->max >> 1;
			step = span>>data->perlin_startdepth;
			pp = 0;
			for (int l = data->perlin_startdepth; l < data->perlin_depth; l++) {
				prw = (1<<l)+1;
				lx = x / step; 
				ly = y / step;
				
				tl = pregen[pp + ly * prw + lx] %d;
				tr = pregen[pp + ly * prw + lx + 1] %d; 
				bl = pregen[pp + (ly+1) * prw + lx] %d;
				br = pregen[pp + (ly+1) * prw + lx + 1] %d;
				
				tx = (float)(x%step) / (float)step;
				ty = (float)(y%step) / (float)step;
				val = val + cerp( cerp(br, bl, tx), cerp(tr, tl, tx), ty );

				d = BT_MAX(1, d>>1);
				step = BT_MAX(1, step>>1);
				side = prw-1;
				pp += side*side;
				
				//what's a better way to generate normal perlin noise?!
			}
			noise[y*w+x] = val;
			biggest = BT_MIN(biggest, val);
		}
	}
	free(pregen);
	printf("%u %u %u\n", data->max >> 1, data->max >> 2);
	return noise;
}


void
perlin_test()
{
	for (int i = 0; i < 10; i++) {
		printf("PRNG %d: %u\n", i, prng(sizeof(unsigned int), i));
	}
	noise_data pd;
	int pdd[2] = {256, 256};
	pd.dims = pdd;
	pd.ndims = 2;
	pd.max = 256;
	pd.type = NOISE_PERLIN;
	pd.perlin_depth = 10;
	tick();
	perlin_generate2dui(&pd);
	tock();
}