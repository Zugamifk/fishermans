void*
noise_generate
(
	int* type,
	noise_data* data
)
{
	switch (data->type) {
		case NOISE_PERLIN: {
			*type = 0;
			//return perlin(data);
		} break;
		default: break;
	}
	
	*type = -1;
	return NULL;
}