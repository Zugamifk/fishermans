void
shapes_ring(double r, double t, int segnum)
{
	double a = 0.0;
	double inc = (2.0*pi)/(double)segnum;
	glBegin(GL_QUAD_STRIP);
	for(int i=0;i<segnum;i++) {
		glVertex2d((r-t)*sin(a), (r-t)*cos(a));
		glVertex2d((r+t)*sin(a), (r+t)*cos(a));
		a+=inc;
	}
	glVertex2d(0.0, r-t);
	glVertex2d(0.0, r+t);
	glEnd();
}

void
shapes_cursor(double s)
{
	glScalef(s, s, s);
	glBegin(GL_POLYGON);
	glVertex2d(0.0, 0.0);
	glVertex2d(0.1, -0.1);
	glVertex2d(0.05, -0.1);
	glVertex2d(0.0, -0.14);
	glEnd();
}