int
math_roll(double chance) {
	double s = (double)(rand()%100000)/100000.0;
	return s < chance;
}

// unit functions
double
math_usin(double x)
{
	return 0.5+0.5*sin(x);
}

double
math_ucos(double x)
{
	return 0.5+0.5*cos(x);
}

// Colour generating functions
void
math_rcol(vec4 *v)
{
	vec4_set(v, 
		0.5-0.5*cos(rand()),
		0.5+0.5*sin(rand()),
		0.5-0.5*sin(rand()),
		0.0	
	);
}

void
math_explodecol(vec4 *v, double t)
{
	vec4_set(v, 
		(0.2-t)/0.1+0.2,
		(0.1-t)/0.1+0.2,
		(0.05-t)/0.05+0.2,
		0.0);
}

void
math_rainbow(vec4 *v, double t)
{
	vec4_set(v,
		math_usin(t*2.0*pi),
		math_ucos(t*2.0*pi),
		math_usin(t*2.0*pi),
		0.0);
}
