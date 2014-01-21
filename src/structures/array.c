float*
init_floats(int num, float init) {
	float* f = malloc(sizeof(float)*num);
	for(int i = 0; i < num; i++) {
		f[i] = init;
	}
	return f;
}