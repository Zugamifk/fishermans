static unsigned int prng(unsigned int word, unsigned int seed)
{
	seed = hashlittle(&seed, word, 0xb00b135);
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

#define PERLIN_LERP(t) 		(t)
#define PERLIN_COSB(t) 		(0.5 - 0.5*cos(t*3.14159))
#define PERLIN_HERMITEB(t) 	(3.0*t*t - 2.0*t*t*t)
#define PERLIN_FIFTHB(t) 	(6.0*t*t*t*t*t - 15.0*t*t*t*t + 10.0*t*t*t)
static unsigned int blend(unsigned int a, unsigned int b, float t) {
	t = PERLIN_FIFTHB(t);
	return (unsigned int)((t*(float)a)+((1.0-t)*(float)b));
}

unsigned int*
perlin_generate2dui
(
	noise_data* data
)
{
	unsigned int word = sizeof(unsigned int); // length of a word
	unsigned int pregenside = (1<<data->perlin_depth)+1; // 2^n + 1 for interpolation
	unsigned int *pregen = malloc(sizeof(unsigned int)*pregenside*pregenside*2); // (sum squares <= n ) <= 2n^2
	unsigned int cursor = 0;
 
	for (int l = data->perlin_startdepth; l < data->perlin_depth; l++) {
		int side = (1 << l) + 1;
		for (int i = 0; i < side*side; i++) {
			pregen[cursor] = prng(word, cursor + data->seed); // pseudo-rng
			cursor++;
		}
	}
	
	// calculate size and allocate space for texture
	unsigned int size = data->dims[0] * data->dims[1];
	unsigned int *noise = malloc(sizeof(unsigned int)*size);
	
	unsigned int val;
	unsigned int range;
	unsigned int	w = data->dims[0], h = data->dims[1],  // texture dims
					x = 0, y = 0, // texture positions
					tl, tr, bl, br, px, py, pstep, prow; // sampling vars
	unsigned int pspan;
	unsigned int lng = BT_MAX(w, h);
	for (pspan = 1; pspan < lng; pspan <<= 1) {} // find out how large the deepest level is
	float tx, ty; // interpolation parameters
	unsigned int pp; // pregenerated prng array cursor
	
	int l;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			val = 0; // value to generate
			range = data->max >> 1; // for a range 0-max = sum x % (1>>n) for n = 1:(max-1) 
			pstep = pspan>>data->perlin_startdepth; // initialize the start step size
			pp = 0; // initialize pregen cursor
			
			// sample each resolution
			for (l = data->perlin_startdepth; l < data->perlin_depth; l++) {
				prow = (1<<l)+1; // length of a row in pregen array = 2^l + 1
				px = x / pstep;  
				py = y / pstep; // scale position for resolution
				
				// array position is:
				//		pregen[ (cursor) + (scaled y coord * row length) + (scaled x coord) ]
				// samples 4 corners in order to interpolate
				tl = pregen[pp + (py * prow) + px] %range;
				tr = pregen[pp + py * prow + px + 1] %range; 
				bl = pregen[pp + (py+1) * prow + px] %range;
				br = pregen[pp + (py+1) * prow + px + 1] %range;
				
				tx = (float)(x%pstep) / (float)pstep;
				ty = (float)(y%pstep) / (float)pstep;
				val = val + blend( blend(br, bl, tx), blend(tr, tl, tx), ty );

				range = BT_MAX(1, range>>1);
				pstep = BT_MAX(1, pstep>>1);
				pp += prow*prow;
				
				//what's a better way to generate normal perlin noise?!
			}
			noise[x*h+y] = val;
		}
	}
	free(pregen);
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