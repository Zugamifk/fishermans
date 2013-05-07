gui_container *
gui_container_init
(double w, double h)
{
	gui_container *c = malloc(sizeof(gui_container));
	c->content = NULL;
	c->type = GUI_CONTENTTYPE_NULL;
	c->visible = 0;
	c->width = w;
	c->height = h;
	return c;
}

gui_container *
gui_container_initwith
(double w, double h, void *content, gui_contenttype type, double x, double y)
{
	gui_container *c = gui_container_init(w, h);
	gui_container_insert(c, content, type, x, y);
	return c;
}

gui_container *
gui_container_initmany
(double w, double h, int axis, void **contents, gui_contenttype *types, vec2 *positions, int num)
{
	if (num < 2) {
		return
		gui_container_initwith(
					1, 1,
					*contents,
					types[0],
					positions[0].x,
					positions[0].y
		);
	} else {
		double pos = 1.0/(double)num;
		return
		gui_container_initwith(w, h,
			gui_div_initwith(
				axis,
				gui_container_initwith(
					1, 1,
					*contents,
					types[0],
					positions[0].x,
					positions[0].y
				),
				gui_container_initmany(1, 1, axis, contents+1, types+1, positions+1, num-1)
			),
			GUI_CONTENTTYPE_DIV,
			pos, 1.0-pos
		);
	}
}

void
gui_container_draw
(gui_container *c, double t, double dt)
{
	if (c == NULL) return;
	switch(c->type) {
		case GUI_CONTENTTYPE_DIV:
			gui_div_draw(c->content, t, dt);
		break;
		case GUI_CONTENTTYPE_BUTTON:
			gui_button_draw(c->content, t, dt);
		break;
		case GUI_CONTENTTYPE_TEXTBOX:
			gui_textbox_draw(c->content, t, dt);
		break;
		case GUI_CONTENTTYPE_SLIDER:
			gui_slider_draw(c->content, t, dt);
		break;
	}
	
	if (c->type != GUI_CONTENTTYPE_DIV) {
		glEnable(GL_TEXTURE_2D); 
		if(c->visible)
			color_apply(gui_colors->colors[5]);
		else
			color_apply(gui_colors->colors[0]);
			
		double l = 0.0, h = 1.0;
		glBegin(GL_QUADS);
		glTexCoord2f(l, l);
		glVertex2d(0.0, 0.0);
		
		glTexCoord2f(h*c->width, l);
		glVertex2d(c->width, 0.0);
		
		glTexCoord2f(h*c->width, h);
		glVertex2d(c->width, c->height);
		
		glTexCoord2f(l, h);
		glVertex2d(0.0, c->height);
		
		glEnd();
		glDisable(GL_TEXTURE_2D); 
	}
}

void
gui_container_resize(gui_container *c, double w, double h)
{
	c->width = w;
	c->height = h;
	switch(c->type) {
		case GUI_CONTENTTYPE_DIV: 
		{
			gui_div *div = (gui_div *)(c->content);
			gui_div_resize(div, w, h);
		} break;
		case GUI_CONTENTTYPE_BUTTON:
		{
			gui_button *button = (gui_button *)(c->content);
			gui_box_position(button->bounds, c, button->scalarx, button->scalary);
		} break;
		case GUI_CONTENTTYPE_TEXTBOX:
		{
			gui_textbox *textbox = (gui_textbox *)(c->content);
			gui_box_position(textbox->bounds, c, textbox->scalarx, textbox->scalary);
		} break;
		case GUI_CONTENTTYPE_SLIDER:
		{
			gui_slider *slider = (gui_slider *)(c->content);
			gui_box_position(slider->bounds, c, slider->pos->x, slider->pos->y);
		}
	}
}

int
gui_container_insert(gui_container *c, void *content, gui_contenttype type, double x, double y)
{
	if (c->type != GUI_CONTENTTYPE_NULL) return 0;
	c->content = content;
	c->type = type;
	
	switch(type) {
		case GUI_CONTENTTYPE_DIV: 
		{
			gui_div *d = (gui_div *)content;
			if (d->axis == GUI_DIV_HORZ) 
				gui_div_position(d, y);
			else
			if (d->axis == GUI_DIV_VERT) 
				gui_div_position(d, x);
		} break;
		case GUI_CONTENTTYPE_BUTTON:
		{
			gui_button *button = (gui_button *)content;
			button->scalarx	= x;
			button->scalary	= y;
		} break;
		case GUI_CONTENTTYPE_TEXTBOX:
		{
			gui_textbox *textbox = (gui_textbox *)content;
			textbox->scalarx	= x;
			textbox->scalary	= y;
		} break;
		case GUI_CONTENTTYPE_SLIDER:
		{
			gui_slider *slider = (gui_slider *)content;
			vec2_set(slider->pos, x, y);
		}
	}
	
	gui_container_resize(c, c->width, c->height);
	return 1;
}

void
gui_container_reset(gui_container *c)
{
	if (c->type == GUI_CONTENTTYPE_DIV)
	{
		gui_div *div = (gui_div *)(c->content);
		if (div->divs[0] != NULL)
			gui_container_reset(div->divs[0]);
		if (div->divs[1] != NULL)
			gui_container_reset(div->divs[1]);
	} else
	if (c->type == GUI_CONTENTTYPE_BUTTON)
	{
		gui_button *button = (gui_button *)(c->content);
		button->state = GUI_BUTTON_ACTIVE;
	}
}

void
gui_container_findmouse(gui_container *c, double x, double y)
{
	switch(c->type) {
		case GUI_CONTENTTYPE_DIV:
		{
			gui_container *divc = gui_div_find(c, &x, &y);
			if (divc != NULL)
				gui_container_findmouse(divc, x, y);
		} break;
		case GUI_CONTENTTYPE_BUTTON:
		{
			gui_button *button = (gui_button *)(c->content);
			if (gui_button_onbutton(c->content, c, x, y))
			{
				if (gui_mousestate->buttonstate == GLUT_DOWN && button->state != GUI_BUTTON_INACTIVE)
					button->state = GUI_BUTTON_CLICKED;
			}
		} break;
		case GUI_CONTENTTYPE_SLIDER:
		{
			gui_slider *slider = (gui_slider *)(c->content);
			if (	gui_mousestate->buttonstate == GLUT_DOWN 
				&& 	slider->state != GUI_SLIDER_INACTIVE
				&&	gui_box_inbox(slider->bounds, x, y))
			{
				gui_slider_onclick(slider, x, y);
			}
		}
	}
}

void
gui_container_mouseup(gui_container *c, double x, double y)
{
	switch(c->type) {
		case GUI_CONTENTTYPE_DIV:
		{
			gui_container *divc = gui_div_find(c, &x, &y);
			if (divc != NULL)
				gui_container_mouseup(divc, x, y);
		} break;
		case GUI_CONTENTTYPE_BUTTON:
		{
			gui_button *button = (gui_button *)(c->content);
			if (gui_button_onbutton(c->content, c, x, y))
			{
				if (button->state == GUI_BUTTON_CLICKED)
				{	
					button->state = GUI_BUTTON_ACTIVE;
					gui_button_triggerevent(button);
				}
			}
		} break;
		case GUI_CONTENTTYPE_SLIDER:
		{
			gui_slider *slider = (gui_slider *)(c->content);
			if (	slider->state != GUI_SLIDER_INACTIVE)
			{
				slider->state = GUI_SLIDER_ACTIVE;
				gui_slider_triggerevent(slider);
			}
		}
	}
}