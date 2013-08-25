void
shapes_box
(
	double w,
	double h
)
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(0.0, 0.0);
	glVertex2d(w, 0.0);
	glVertex2d(w, h);
	glVertex2d(0.0, h);
	glEnd();
}
void
shapes_boxfilled
(
	double w,
	double h
)
{
	glBegin(GL_POLYGON);
	glVertex2d(0.0, 0.0);
	glVertex2d(w, 0.0);
	glVertex2d(w, h);
	glVertex2d(0.0, h);
	glEnd();
}

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
shapes_cursor()
{
	//glScalef(s, s, s);
	glBegin(GL_POLYGON);
	glVertex2d(0.0, 0.0);
	glVertex2d(20, -20);
	glVertex2d(8, -15);
	glVertex2d(0.0, -30);
	glEnd();
}