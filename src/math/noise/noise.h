typedef enum
_noise_type_e
{
	NOISE_BAD = -1,
	NOISE_STATIC,
	NOISE_PERLIN
} noise_type;

typedef enum
_noise_ipol_cb_e
{
	NOISE_LINEAR,
	NOISE_COS,
	NOISE_BEZIER
} noise_ipol_cb;

typedef struct
_noise_data_s
{
	int* dims;
	int ndims;
	unsigned int min;
	unsigned int max;
	noise_type type;
	unsigned int seed;
	unsigned int perlin_depth;
	unsigned int perlin_startdepth;
	unsigned int perlin_persist;
} noise_data;

#include "perlin.c"

#include "noise.c"