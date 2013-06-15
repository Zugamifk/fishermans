gui_button *	
gui_button_init(double s)
{
	gui_button *b = malloc(sizeof(gui_button));
	b->state	= GUI_BUTTON_INACTIVE;
	b->scale	= s;
	b->bounds 	= gui_box_init(0, 0, s, s);
	b->text		= NULL;
	b->scalarx	= 0.0;
	b->scalary	= 0.0;
	b->font 	= NULL;
	b->clickevent= EVENT_ID_NULL;
	b->eventdata= NULL;
	return b;
}

gui_button *	
gui_button_initwithf(char *text, fontinfo *fi, double s)
{
	gui_button *b = malloc(sizeof(gui_button));
	b->state	= GUI_BUTTON_INACTIVE;
	b->scale	= s;
	b->bounds 	= gui_box_init(0, 0, font_getstrwidth(text, fi)*s, s);
	b->text		= text;
	b->scalarx	= 0.0;
	b->scalary	= 0.0;
	b->font 	= fi;
	b->clickevent= EVENT_ID_NULL;
	b->eventdata= NULL;
	return b;
}

gui_button *	
gui_button_initwithe(event_id e, void *data, void (*f)(void *), double s)
{
	gui_button *b = malloc(sizeof(gui_button));
	b->state	= GUI_BUTTON_ACTIVE;
	b->scale	= s;
	b->bounds 	= gui_box_init(0, 0, s, s);
	b->text		= NULL;
	b->scalarx	= 0.0;
	b->scalary	= 0.0;
	b->font 	= NULL;
	b->clickevent= e;
	b->eventdata= data;
	bus_subscribe(gui_eventbus, b->clickevent, f);
	return b;
}


gui_button *
gui_button_initwithfe(char *text, fontinfo *fi, event_id e, void *data, void (*f)(void *), double s )
{
	gui_button *b = gui_button_initwithf(text, fi, s);
	gui_button_setevent(b, e, data);
	bus_subscribe(gui_eventbus, b->clickevent, f);
	return b;
}

int			
gui_button_setevent(gui_button *b, event_id e, void *data){
	if (b->clickevent != EVENT_ID_NULL) 
		return 0;
		
	b->clickevent = e;
	b->eventdata = data;
	b->state = GUI_BUTTON_ACTIVE;
	return 1;
}

void
gui_button_triggerevent(gui_button *b)
{
	if (b->clickevent != EVENT_ID_NULL)
		bus_triggerevent(gui_eventbus, b->clickevent, b->eventdata);
}

int
gui_button_onbutton(gui_button *b, gui_container *c, double x, double y)
{
	return gui_box_inbox(b->bounds, x, y);
}

void			
gui_button_draw(gui_button *b, double t, double dt)
{
	gui_box *box = b->bounds;

	glPushMatrix();	
	gui_box_translateinto(box);
	glScaled(b->scale*box->scale, b->scale*box->scale, 1.0);
	color_apply(gui_colors->colors[2]);
	if (b->font != NULL) {
		font_draw(b->text, b->font, 0.0, 0.0, 1.0, FONT_LEN_INF);
	}
	glPopMatrix();
	
	gui_box_draw(box, GL_LINE_LOOP, gui_colors->colors[1]);
	
	color *col = gui_colors->colors[3];
	if	(b->state == GUI_BUTTON_CLICKED) col = gui_colors->colors[4];
	gui_box_draw(box, GL_QUADS, col);
}
 