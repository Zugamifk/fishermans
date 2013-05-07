typedef struct
vec4 {
	double x;
	double y;
	double z;
	double w;
} vec4;
vec4 *vec_0, *vec_up, *vec_forward, *vec_right;

// Data shit
void 
vec4_print(vec4 *v) 
{
	printf("[%f, %f, %f, %f]\n", v->x, v->y, v->z, v->w);
}

vec4 * 
vec4_new(double x, double y, double z, double w) 
{
	vec4 *v = malloc(sizeof(vec4));
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
	return v;
}

void
vec4_set(vec4 *v, double x, double y, double z, double w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void
vec4_initglobals()
{
	vec_0 	= vec4_new(0.0, 0.0, 0.0, 0.0);
	vec_right 	= vec4_new(1.0, 0.0, 0.0, 0.0);
	vec_up 	= vec4_new(0.0, 1.0, 0.0, 0.0);
	vec_forward = vec4_new(0.0, 0.0, 1.0, 0.0);
}

void
vec4_setv(vec4 *v, vec4 *f)
{
	v->x = f->x;
	v->y = f->y;
	v->z = f->z;
	v->w = f->w;
}

vec4 *
vec4_0()
{
	return vec4_new(0.0, 0.0, 0.0, 0.0);
}

vec4 *
vec4_copy(vec4 *v) {
	vec4 *cp = malloc(sizeof(vec4));
	cp->x = v->x;
	cp->y = v->y;
	cp->z = v->z;
	cp->w = v->w;
	return cp;
}

void
vec4_copyto(vec4 *cp, vec4 *v) {
	cp->x = v->x;
	cp->y = v->y;
	cp->z = v->z;
	cp->w = v->w;
}

void
vec2_vtoa(double *res, vec4 *v) {
	res[0] = v->x;
	res[1] = v->y;
}

void
vec3_vtoa(double *res, vec4 *v) {
	res[0] = v->x;
	res[1] = v->y;
	res[2] = v->z;
}

void
vec4_vtoa(double *res, vec4 *v) {
	res[0] = v->x;
	res[1] = v->y;
	res[2] = v->z;
	res[3] = v->w;
}

// Opengl interfacing
void
vec4_apply(vec4 *v) 
{
	glVertex3d(v->x, v->y, v->z);
} 

void
vec4_translate(vec4 *v)
{
	glTranslated(v->x, v->y, v->z);
}

void
vec4_color(vec4 *v)
{
	glColor3f(v->x, v->y, v->z);
}

// Lower dimensional vector math
void 
vec2_rotate(vec2 *res, vec2 *v, double a) 
{
	res->x = v->x*cos(a) - v->y*sin(a);
	res->y = v->x*sin(a) + v->y*cos(a);	
}

void
vec3_cross(vec3 *res, vec3 *a, vec3 *b) 
{
	res->x = a->y*b->z - a->z*b->y;
	res->y = a->z*b->x - a->x*b->z;
	res->z = a->x*b->y - a->y*b->x;
}

// Math
void 
vec4_add(vec4 *res, vec4 *a, vec4 *b) 
{
	res->x = a->x + b->x;
	res->y = a->y + b->y;
	res->z = a->z + b->z;
	res->w = a->w + b->w;
}

void 
vec4_addto(vec4 *a, vec4 *b) 
{
	a->x = a->x + b->x;
	a->y = a->y + b->y;
	a->z = a->z + b->z;
	a->w = a->w + b->w;
}

void 
vec4_sub(vec4 *res, vec4 *a, vec4 *b) 
{
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	res->z = a->z - b->z;
	res->w = a->w - b->w;
}

void 
vec4_subto(vec4 *a, vec4 *b) 
{
	a->x = a->x - b->x;
	a->y = a->y - b->y;
	a->z = a->z - b->z;
	a->w = a->w - b->w;
}

void 
vec4_scale(vec4 *res, vec4 *v, double s) 
{
	res->x = v->x * s;
	res->y = v->y * s;
	res->z = v->z * s;
	res->w = v->w * s;
}

void 
vec4_scaleto(vec4 *v, double s) 
{
	v->x = v->x * s;
	v->y = v->y * s;
	v->z = v->z * s;
	v->w = v->w * s;
}

double 
vec4_length(vec4 *v) 
{
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z + v->w*v->w);
}

void 
vec4_normalize(vec4 *res, vec4 *v) 
{
	double len = vec4_length(v);
	vec4_scale(res, v, 1.0/len);
}

void 
vec4_normalizeto(vec4 *v) 
{
	double len = vec4_length(v);
	vec4_scaleto(v, 1.0/len);
}

double 
vec4_dot(vec4 *a, vec4 *b) 
{
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

void
vec4_mult(vec4 *res, vec4 *a, vec4 *b) 
{
	vec4 u1, u2;
	vec4_copyto(&u1, a);
	vec4_copyto(&u2, b);
	u1.w = 0.0;
	u2.w = 0.0;

	vec4 v1, v2, v3;
	vec4_scale(&v1, &u1, b->w);
	vec4_scale(&v2, &u2, a->w);
	vec3_cross(&v3, &u1, &u2);
	
	res->x = v1.x + v2.x + v3.x;
	res->y = v1.y + v2.y + v3.y;
	res->z = v1.z + v2.z + v3.z;
	res->w = a->w * b->w - vec4_dot(&u1, &u2);
}

void
vec4_conj(vec4 *res, vec4 *v) 
{
	res->x = v->x*-1.0;
	res->y = v->y*-1.0;
	res->z = v->z*-1.0;
	res->w = v->w;
}

void
vec4_rotate(vec4 *res, vec4 *v, vec4 *u, double a) 
{
	float c = cos(a/2.0);
	float s = sin(a/2.0);
	
	vec4 q, qc;
	q.x =	s*u->x;
	q.y = s*u->y;
	q.z = s*u->z;
	q.w = c;
	vec4_conj(&qc, &q);
	
	vec4_mult(res, &q, v);
	vec4_mult(res, res, &qc);
}

void
vec4_rotateq(vec4 *res, vec4 *v, vec4 *q) 
{
	vec4 qc;
	vec4_conj(&qc, q);
	
	vec4_mult(res, q, v);
	vec4_mult(res, res, &qc);
}

void
vec4_rotateqa(vec4 *q) 
{
	double a = acos(q->w);
	if (a!=0) {
		vec4 qa;
		vec4_scale(&qa, q, sin(a/2.0));
		glRotated(a*180.0/pi*2.0, qa.x, qa.y, qa.z);
	}
}

void
vec4_slerpv(vec4 *res, vec4 *q1, vec4 *q2, double a)
{
	double phi1 = acos(vec4_dot(q1, q2));
	vec4 q2c;
	vec4_conj(&q2c, q2);
	double phi2 = acos(vec4_dot(q1, &q2c));
	double phi = phi1>phi2 ? phi2 : phi1;

	double s1 = sin((1.0-a)*phi)/sin(phi);
	double s2 = sin(a*phi)/sin(phi);

	vec4 sq1, sq2;
	vec4_scale(&sq1, q1, s1);
	vec4_scale(&sq2, q2, s2);
	vec4_add(res, &sq1, &sq2);
}

// ==============================
