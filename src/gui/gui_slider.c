gui_slider *
gui_slider_init
(double l, double w, int a)
{
	gui_slider *s = malloc(sizeof(gui_slider));
	s->state	= GUI_SLIDER_ACTIVE;
	s->axis		= a;
	s->scale	= w;
	s->length	= l;
	s->val 		= 0.0;
	if (a == GUI_SLIDER_HORZ) {
		s->bounds	= gui_box_init(0, 0, l+w, w);
	} else if (a == GUI_SLIDER_VERT) {
		s->bounds = gui_box_init(0, 0, w, l+w);
	}
	s->pos		= vec2_new(0, 0);
	s->slideevent = EVENT_ID_NULL;
	return s;
}

gui_slider *
gui_slider_initwithe
(double l, double w, int a, event_id e, void (*f)(void *))
{
	gui_slider *s = gui_slider_init(l, w, a);
	gui_slider_setevent(s, e);
	bus_subscribe(gui_eventbus, s->slideevent, f);
	return s;
}

int
gui_slider_setevent
(gui_slider *s, event_id e)
{
	if (s->slideevent != EVENT_ID_NULL)
		return 0;
	
	s->slideevent = e;
	return 1;
}

void
gui_slider_triggerevent
(gui_slider *s)
{
	if (s->slideevent != EVENT_ID_NULL)
		bus_triggerevent(gui_eventbus, s->slideevent, &(s->val));
}

void
gui_slider_onclick(gui_slider *s, double x, double y)
{
	double dx;
	gui_box *b = s->bounds;
	if (s->axis == GUI_SLIDER_HORZ)
	{
		double pad = b->scale * (b->hpadin + s->scale/2.0);
		if (x < b->x+pad) 
			dx = 0.0;
		else if (x > b->x + b->scale*b->width-pad)
			dx = 1.0;
		else
			dx = (x - b->x - pad) / (b->scale*b->width - 2.0*pad);
		s->val = dx;
	} else
	if (s->axis == GUI_SLIDER_VERT)
	{
		double pad = b->scale * (b->vpadin + s->scale/2.0);
		if (y < b->y+pad) 
			dx = 0.0;
		else if (y > b->y + b->scale*b->height-pad)
			dx = 1.0;
		else
			dx = (y - b->y - pad) / (b->scale*b->height - 2.0*pad);
		s->val = dx;
	}
	gui_slider_triggerevent(s);
}

void
gui_slider_draw
(gui_slider *s, double t, double dt)
{
	glPushMatrix();
		gui_box_translateinto(s->bounds);
		glScaled(s->bounds->scale, s->bounds->scale, 1.0);
		color_apply(gui_colors->colors[1]);
		double os = s->scale/2.0;
		double ex = s->bounds->hpadin;
		double ey = s->bounds->vpadin;
		double p = os + s->length * s->val;
		
		if (s->axis == GUI_SLIDER_HORZ) 
		{	
			glBegin(GL_POLYGON);
				glVertex2d(p-os-ex, -ey);
				glVertex2d(p-os-ex, os*2.0+ey);
				glVertex2d(p+os+ex, os*2.0+ey);
				glVertex2d(p+os+ex, -ey);
			glEnd();
			glBegin(GL_LINES);
				glVertex2d(os, os);
				glVertex2d(s->length+os, os);
			glEnd();
		} else
		if (s->axis == GUI_SLIDER_VERT) 
		{
			glBegin(GL_POLYGON);
				glVertex2d(-ex, p-os-ey);
				glVertex2d(os*2.0+ex, p-os-ey);
				glVertex2d(os*2.0+ex, p+os+ey);
				glVertex2d(-ex, p+os+ey);
			glEnd();
			glBegin(GL_LINES);
				glVertex2d(os, os);
				glVertex2d(os, s->length+os);
			glEnd();
		}
	glPopMatrix();
	
	//gui_box_draw(s->bounds, GL_LINE_LOOP, gui_colors->colors[1]);

}