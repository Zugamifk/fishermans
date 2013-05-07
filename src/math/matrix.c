typedef struct
matrix
{
	double *m;
	int w;
	int h;
} matrix;


void
matrix_print(matrix *m)
{
	for(int y=0;y<m->h;y++) {
		printf("[ ");
		for(int x=0;x<m->w;x++) {
			int p = m->w*y + x;
			printf("%f ", m->m[p]);
		}
		printf("]\n");
	}
}

void
matrix_scale(matrix *res, matrix *m, double s)
{
	if (res->w != m->w || res->h != m->h) return;

	for(int y=0;y<m->h;y++) {
		for(int x=0;x<m->w;x++) {
			int p = m->w*y + x;
			res->m[p] = m->m[p]*s;
		}
	}
}
 
void
matrix_add(matrix *res, matrix *a, matrix *b)
{
	if (a->w != b->w || a->h != b->h) return;

	for(int y=0;y<res->h;y++) {
		for(int x=0;x<res->w;x++) {
			int p = res->w*y + x;
			res->m[p] = a->m[p] + b->m[p];
		}
	}
}

void
matrix_mult(matrix *res, matrix *a, matrix *b)
{

	if (a->w != b->h)  {
		printf("Can not multiply matrix of width %d with a matrix of height %d!\n", a->w, b->h);
	}	
	
	for(int x=0;x<res->w;x++) {
		for(int y=0;y<res->h;y++) {
			int p = res->w*y + x;
			res->m[p] = 0.0;
			for(int i=0;i<a->w;i++) {
				int pa = a->w*y + i;
				int pb = b->w*i + x;
				res->m[p] += a->m[pa] * b->m[pb];
			}
		}
	}	
}

void
matrix_transpose(matrix *res, matrix *m)
{
	for(int x=0;x<res->w;x++) {
		for(int y=0;y<res->h;y++) {
			int p = res->w*y + x;
			int pt = res->h*x + y;
			res->m[p] = m->m[pt];
		}
	}	
}

void
matrix_mulvec(double *res, matrix *m, double *v, int vs)
{
	if (vs!=m->w) {
		printf("Vector size %d can't multiply with matrix of size %d!\n", vs, m->w);	
		return;
	}

	for (int i=0;i<m->h;i++) {
		res[i] = 0.0;
		for(int j=0;j<vs;j++) {
			int p = m->w*i + j;
			res[i] += v[j]*m->m[p];
		}
	}

}
