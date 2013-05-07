gui_div * 
gui_div_init(int axis)
{
	gui_div *div = malloc(sizeof(gui_div));
	div->axis = axis;
	div->scalarpos = 0.5;
	div->realpos = 0.5;
	div->width = 1.0;
	div->height = 1.0;
	div->visible = 1;
	div->divs = malloc(sizeof(gui_container *)*2);
	div->divs[0] = NULL;
	div->divs[1] = NULL;
	return div;
}

gui_div*
gui_div_initwith(int axis, gui_container *c0, gui_container *c1)
{
	gui_div *d = gui_div_init(axis);
	gui_div_insert(d, 0, c0);
	gui_div_insert(d, 1, c1);
	return d;
}

void
gui_div_position(gui_div *d, double spos)
{
	d->scalarpos = spos;
	if (d->axis == GUI_DIV_HORZ)
		d->realpos	= spos * d->height;
	else
	if (d->axis == GUI_DIV_VERT)
		d->realpos = spos * d->width;
}

int
gui_div_insert(gui_div *d, int pos, gui_container *c)
{
	if (d->divs[pos] != NULL || c == NULL)
		return 0;
	
	d->divs[pos] = c;
	gui_div_resize(d, d->width, d->height);
	return 1;
}

void
gui_div_resize(gui_div *div, double w, double h)
{	
	div->width = w;
	div->height = h;
	if (div->axis == GUI_DIV_HORZ) {
		div->realpos = h * div->scalarpos;
		if (div->divs[0] != NULL)
			gui_container_resize(div->divs[0], w, h - div->realpos);
		if (div->divs[1] != NULL)
			gui_container_resize(div->divs[1], w, div->realpos);
	} else
	if (div->axis == GUI_DIV_VERT) {
		div->realpos = w * div->scalarpos;
		if (div->divs[0] != NULL)
			gui_container_resize(div->divs[0], div->realpos, h);
		if (div->divs[1] != NULL)
			gui_container_resize(div->divs[1], w - div->realpos, h);
	}
}

gui_container *
gui_div_find(gui_container *c, double *x, double *y)
{
	gui_div *div = (gui_div *)(c->content);
	if (div->axis == GUI_DIV_HORZ) {
		if (div->divs[GUI_DIV_BOTTOM] != NULL && *y < div->realpos)
			return div->divs[GUI_DIV_BOTTOM];
		else
		if (div->divs[GUI_DIV_TOP] != NULL && *y >= div->realpos)
			*y -= div->realpos;
			return div->divs[GUI_DIV_TOP];
		} else
	if (div->axis == GUI_DIV_VERT) {
		if (div->divs[GUI_DIV_LEFT] != NULL && *x < div->realpos)
			return div->divs[GUI_DIV_LEFT];
		else
		if (div->divs[GUI_DIV_RIGHT] != NULL && *x >= div->realpos)
			*x -= div->realpos;
			return div->divs[GUI_DIV_RIGHT];
	}
	return NULL;
}

void
gui_div_draw(gui_div *d, double t, double dt)
{

	if	(d->axis == GUI_DIV_HORZ) 
	{
		glPushMatrix();
			if (d->visible) {
				color_apply(gui_colors->colors[1]);
				glBegin(GL_LINES);
				glVertex2d(0.0, d->realpos);
				glVertex2d(d->width, d->realpos);
				glEnd();
			}
			gui_container_draw(d->divs[GUI_DIV_BOTTOM], t, dt);
			glTranslated(0.0, d->realpos, 0.0);
			gui_container_draw(d->divs[GUI_DIV_TOP], t, dt);
		glPopMatrix();
	} else
	if	(d->axis == GUI_DIV_VERT) 
	{
		glPushMatrix();
			if (d->visible) {
				color_apply(gui_colors->colors[1]);
				glBegin(GL_LINES);
				glVertex2d(d->realpos, 0.0);
				glVertex2d(d->realpos, d->height);
				glEnd();
			}
			gui_container_draw(d->divs[GUI_DIV_LEFT], t, dt);
			glTranslated(d->realpos, 0.0, 0.0);
			gui_container_draw(d->divs[GUI_DIV_RIGHT], t, dt);
		glPopMatrix();
	}
}
