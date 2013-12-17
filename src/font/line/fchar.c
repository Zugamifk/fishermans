fontchar *
fontchar_init()
{
	fontchar *fc = malloc(sizeof(fontchar));
	return fc;
}

void
fontchar_tostring(fontchar *fc)
{
	printf("%d PTS:\n", fc->ptct);
	for(int i=0;i<fc->ptct;i++) {
		vec4_print(fc->pts[i]);
	}
	printf("%d EDGES:\n", fc->edgect);
	for(int i=0;i<fc->edgect;i++) {
		vec4_print(fc->edges[i][0]);
		vec4_print(fc->edges[i][1]);
	}
	printf("WIDTH: %lf\n", fc->width);
}

void
fontchar_draw(fontchar *fc)
{
	if (fc) {
		glBegin(GL_LINES);
		for(int i = 0;i<fc->edgect;i++) {
			glVertex2d(fc->edges[i][0]->x, fc->edges[i][0]->y);
			glVertex2d(fc->edges[i][1]->x, fc->edges[i][1]->y);
		}
		glEnd();
	} else {
		glBegin(GL_LINE_LOOP);
			glVertex2d(0.0, 0.0);
			glVertex2d(0.0, 1.0);
			glVertex2d(0.9, 1.0);
			glVertex2d(0.9, 0.0);
		glEnd();
	}
}
